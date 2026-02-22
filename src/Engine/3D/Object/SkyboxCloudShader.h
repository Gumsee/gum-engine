#pragma once
#include <Graphics/Shader.h>

static const std::string CloudFragmentShader = GLSL(
    in vec2 Texcoord;
    in mat4 projection;
    in mat4 view;
    out vec4 FragColor;

    uniform float time;
    uniform vec3 camPos;
    uniform vec3 camDir;
    uniform vec3 sunDir;
    uniform ivec2 canvassize;
    uniform sampler3D noise;

    const float RENDER_DISTANCE = 5000;

    const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                        -0.80,  0.36, -0.48,
                        -0.60, -0.48,  0.64 );
    const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                        0.80,  0.36, -0.48,
                        0.60, -0.48,  0.64 );

    float hash1( float n )
    {
        return fract( n*17.0*fract( n*0.3183099 ) );
    }

    vec4 noised( in vec3 x )
    {
        vec3 p = floor(x);
        vec3 w = fract(x);
        #if 1
        vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
        vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);
        #else
        vec3 u = w*w*(3.0-2.0*w);
        vec3 du = 6.0*w*(1.0-w);
        #endif

        float n = p.x + 317.0*p.y + 157.0*p.z;
        
        float a = hash1(n+0.0);
        float b = hash1(n+1.0);
        float c = hash1(n+317.0);
        float d = hash1(n+318.0);
        float e = hash1(n+157.0);
        float f = hash1(n+158.0);
        float g = hash1(n+474.0);
        float h = hash1(n+475.0);

        float k0 =   a;
        float k1 =   b - a;
        float k2 =   c - a;
        float k3 =   e - a;
        float k4 =   a - b - c + d;
        float k5 =   a - c - e + g;
        float k6 =   a - b - e + f;
        float k7 = - a + b + c - d + e - f - g + h;

        return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                        2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                        k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                        k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
    }


    vec4 fbmd_8( in vec3 x )
    {
        float f = 2.0;
        float s = 0.65;
        float a = 0.0;
        float b = 0.5;
        vec3  d = vec3(0.0);
        mat3  m = mat3(1.0,0.0,0.0,
                    0.0,1.0,0.0,
                    0.0,0.0,1.0);
        for( int i=0; i<8; i++ )
        {
            vec4 n = noised(x);
            a += b*n.x;          // accumulate values		
            if( i<4 )
            d += b*m*n.yzw;      // accumulate derivatives
            b *= s;
            x = f*m3*x;
            m = f*m3i*m;
        }
        return vec4( a, d );
    }

    vec4 cloudsFbm( in vec3 pos )
    {
        return fbmd_8(pos*0.0015+vec3(2.0,1.1,1.0)+0.07 * vec3(time,0.5*time,-0.15*time));
        //return texelFetch(noise, ivec3(pos*0.0015+vec3(2.0,1.1,1.0)+0.07 * vec3(time,0.5*time,-0.15*time)), 0);
    }

    vec4 cloudsMap( in vec3 pos, out float nnd )
    {
        float d = abs(pos.y-900.0)-40.0;
        vec3 gra = vec3(0.0,sign(pos.y-900.0),0.0);
        
        vec4 n = cloudsFbm(pos);
        d += 400.0*n.x * (0.7+0.3*gra.y);
        
        if( d>0.0 ) return vec4(-d,0.0,0.0,0.0);
        
        nnd = -d;
        d = min(-d/100.0,0.25);
        
        gra += 0.1*n.yzw *  (0.7+0.3*gra.y);
        
        return vec4( d, gra );
    }


    vec4 renderClouds( in vec3 ro, in vec3 rd, float tmin, float tmax, inout float resT, in vec2 px )
    {
        vec4 sum = vec4(0.0);

        // bounding volume!!
        float tl = (600.0-ro.y)/rd.y;
        float th = (1200.0-ro.y)/rd.y;
        if(tl > 0.0) tmin = max( tmin, tl ); else return sum;
        if(th > 0.0) tmax = min( tmax, th );

        float t = tmin;
        float lastT = -1.0;
        float thickness = 0.0;
        for(int i = 0; i < 128; i++)
        { 
            vec3  pos = ro + t * rd; 
            float nnd;
            vec4  denGra = cloudsMap( pos, nnd ); 
            float den = denGra.x;
            float dt = max(0.2,0.011*t);
            if( den>0.001 ) 
            { 
                float kk;
                cloudsMap(pos - sunDir * 70.0, kk);
                float sha = 1.0 - smoothstep(-200.0,200.0, kk);
                sha *= 1.5;
                
                vec3 nor = normalize(denGra.yzw);
                float dif = clamp( 0.8 + 0.2 * dot(nor, -sunDir), 0.0, 1.0) * sha; 
                //float fre = clamp( 1.0+dot(nor,rd), 0.0, 1.0 )*sha;
                float occ = 0.3 + 0.6 * max(1.0-kk/200.0,0.0) + 0.1 * (1.0 - den);

                // lighting
                vec3 lin = vec3(0.0);
                lin += vec3(0.70, 0.80, 1.00) * 1.0 * (0.5 + 0.5 * nor.y) * occ;
                lin += vec3(0.10, 0.40, 0.20) * 1.0 * (0.5 - 0.5 * nor.y) * occ;
                lin += vec3(1.00,0.95,0.85) * 3.0 * dif * occ + 0.1;
                

                // color
                vec3 col = vec3(0.8,0.8,0.8)*0.45;
                col = vec3(1.2);

                col *= lin;


                // front to back blending    
                float alp = clamp(den * 0.5 * 0.125 * dt, 0.0, 1.0);
                col.rgb *= alp;
                sum = sum + vec4(col,alp)*(1.0-sum.a);

                thickness += dt*den;
                if( lastT<0.0 ) lastT = t;            
            }
            else 
            {
                dt = abs(den)+0.2;
            }

            t += dt;
            if(sum.a > 0.995 || t > tmax) 
                break;
        }
        
        if(lastT > 0.0)
            resT = min(resT,lastT);

        sum.xyz += max(0.0, 1.0 - 0.0125 * thickness) * vec3(1.00, 0.60, 0.40) * 0.3 * pow(clamp(dot(-sunDir, rd), 0.0, 1.0), 32.0);

        //sum.a = clamp(sum.a, 0.0, 1.0);
        sum.a *= resT / tmax;
        sum.a *= 1 - (resT / (RENDER_DISTANCE * 0.8));
        sum.a = clamp(sum.a, 0.0, 1.0);
        
        //return vec4(vec3(), 1);
        return sum; //clamp( sum, 0.0, 1.0 );
    }

    mat3 setCamera(in vec3 ro, in vec3 ta, float cr)
    {
        vec3 cw = normalize(ta-ro);
        vec3 cp = vec3(sin(cr), cos(cr),0.0);
        vec3 cu = normalize( cross(cw,cp) );
        vec3 cv = normalize( cross(cu,cw) );
        return mat3( cu, cv, cw );
    }

    void main()
    {    
        vec2 p = (2.0 * gl_FragCoord.xy - canvassize.xy) / canvassize.y;

        // camera
        vec3 ro = camPos;

        // ray
        vec3 target = camPos + camDir;
        mat3 cam = setCamera(camPos, target, 0);
        vec3 rd = cam * normalize(vec3(p, 1.5));

        float resT = RENDER_DISTANCE;
        FragColor = renderClouds(ro, rd, 0.0, resT, resT, gl_FragCoord.xy);
    }
);