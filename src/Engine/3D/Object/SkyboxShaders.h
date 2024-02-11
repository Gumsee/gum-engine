#pragma once
#include <Graphics/Shader.h>

static const std::string SkyboxVertexShader = GLSL(
	in vec3 vertexPosition;

	out vec3 Texcoord;

	uniform mat4 projectionMatrix;
	uniform mat4 transformationMatrix;
	uniform mat4 viewMatrix;

	void main()
    {
		mat4 realViewMatrix = viewMatrix * transformationMatrix;
		realViewMatrix[3][0] = 0;
		realViewMatrix[3][1] = 0;
		realViewMatrix[3][2] = 0;
		gl_Position = projectionMatrix * realViewMatrix * vec4(vertexPosition, 1.0f);
		Texcoord = vertexPosition;
	}
);

/*static const std::string SkyboxFragmentShader = GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	uniform samplerCube environmentMap;
	
	void main() 
	{
        vec3 envColor = texture(environmentMap, Texcoord).rgb;
        FragColor = vec4(envColor, 1.0f);
	}
);*/

/*static const std::string SkyboxFragmentShader = GLSL(
    in vec3 Texcoord;
    vec3 fsun = vec3(1);
    out vec4 color;
    uniform float time = 0.0;
    uniform float cirrus = 0.4;
    uniform float cumulus = 0.8;

    const float Br = 0.0025;
    const float Bm = 0.0003;
    const float g  = 0.9800;
    const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
    const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
    const vec3 Km = Bm / pow(nitrogen, vec3(0.84));

    float hash(float n)
    {
        return fract(sin(n) * 43758.5453123);
    }

    float noise(vec3 x)
    {
        vec3 f = fract(x);
        float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
        return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
                    mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
                mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
                    mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
    }

    const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
    float fbm(vec3 p)
    {
        float f = 0.0;
        f += noise(p) / 2; p = m * p * 1.1;
        f += noise(p) / 4; p = m * p * 1.2;
        f += noise(p) / 6; p = m * p * 1.3;
        f += noise(p) / 12; p = m * p * 1.4;
        f += noise(p) / 24;
        return f;
    }

    void main()
    {
        // Atmosphere Scattering
        float mu = dot(normalize(Texcoord), normalize(fsun));
        float rayleigh = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu);
        vec3 mie = (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm);

        vec3 day_extinction = exp(-exp(-((Texcoord.y + fsun.y * 4.0) * (exp(-Texcoord.y * 16.0) + 0.1) / 80.0) / Br) * (exp(-Texcoord.y * 16.0) + 0.1) * Kr / Br) * exp(-Texcoord.y * exp(-Texcoord.y * 8.0 ) * 4.0) * exp(-Texcoord.y * 2.0) * 4.0;
        vec3 night_extinction = vec3(1.0 - exp(fsun.y)) * 0.2;
        vec3 extinction = mix(day_extinction, night_extinction, -fsun.y * 0.2 + 0.5);
        color.rgb = rayleigh * mie * extinction;

        // Cirrus Clouds
        float density = smoothstep(1.0 - cirrus, 1.0, fbm(Texcoord.xyz / Texcoord.y * 2.0 + time * 0.05)) * 0.3;
        color.rgb = mix(color.rgb, extinction * 4.0, density * max(Texcoord.y, 0.0));

        // Cumulus Clouds
        for (int i = 0; i < 3; i++)
        {
            float density = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * Texcoord.xyz / Texcoord.y + time * 0.3));
            color.rgb = mix(color.rgb, extinction * density * 5.0, min(density, 1.0) * max(Texcoord.y, 0.0));
        }

        // Dithering Noise
        //color.rgb += noise(Texcoord * 1000) * 0.01;
    }
);*/


static const std::string SkyboxFragmentShader = GLSL(
    in vec3 Texcoord;
    out vec4 FragColor;

    uniform ivec2 canvassize;

    float iTime = 0.0;

    const float SCENE_SCALE = (10.);
    const float INV_SCENE_SCALE = (.1);

    const vec3 SUN_DIR = normalize(vec3(-.7,.5,.75));
    const vec3 SUN_COLOR = (vec3(1.,.9,.85)*1.4);

    const float HEIGHT_BASED_FOG_B = 0.02;
    const float HEIGHT_BASED_FOG_C = 0.05;

    const int CLOUD_MARCH_STEPS = 12;
    const int CLOUD_SELF_SHADOW_STEPS = 6;

    const float EARTH_RADIUS =    (1500000.); // (6371000.)
    const float CLOUDS_BOTTOM =   (1350.);
    const float CLOUDS_TOP =      (2350.);

    const float CLOUDS_LAYER_BOTTOM =   (-150.);
    const float CLOUDS_LAYER_TOP =      (-70.);

    const float CLOUDS_COVERAGE = (.52);
    const float CLOUDS_LAYER_COVERAGE = (.41);

    const float CLOUDS_DETAIL_STRENGTH = (.225);
    const float CLOUDS_BASE_EDGE_SOFTNESS = (.1);
    const float CLOUDS_BOTTOM_SOFTNESS = (.25);
    const float CLOUDS_DENSITY = (.03);
    const float CLOUDS_SHADOW_MARGE_STEP_SIZE = (10.);
    const float CLOUDS_LAYER_SHADOW_MARGE_STEP_SIZE = (4.);
    const float CLOUDS_SHADOW_MARGE_STEP_MULTIPLY = (1.3);
    const float CLOUDS_FORWARD_SCATTERING_G = (.8);
    const float CLOUDS_BACKWARD_SCATTERING_G = (-.2);
    const float CLOUDS_SCATTERING_LERP = (.5);

    const vec3 CLOUDS_AMBIENT_COLOR_TOP = (vec3(149., 167., 200.)*(1.5/255.));
    const vec3 CLOUDS_AMBIENT_COLOR_BOTTOM = (vec3(39., 67., 87.)*(1.5/255.));
    const float CLOUDS_MIN_TRANSMITTANCE = .1;

    float CLOUDS_BASE_SCALE = 1.51;
    float CLOUDS_DETAIL_SCALE = 20.;

    vec3 getSkyColor(vec3 rd) {
        float sundot = clamp(dot(rd,SUN_DIR),0.0,1.0);
        vec3 col = vec3(0.2,0.5,0.85)*1.1 - max(rd.y,0.01)*max(rd.y,0.01)*0.5;
        col = mix( col, 0.85*vec3(0.7,0.75,0.85), pow(1.0-max(rd.y,0.0), 6.0) );

        col += 0.25*vec3(1.0,0.7,0.4)*pow( sundot,5.0 );
        col += 0.25*vec3(1.0,0.8,0.6)*pow( sundot,64.0 );
        col += 0.20*vec3(1.0,0.8,0.6)*pow( sundot,512.0 );
        
        col += clamp((0.1-rd.y)*10., 0., 1.) * vec3(.0,.1,.2);
        col += 0.2*vec3(1.0,0.8,0.6)*pow( sundot, 8.0 );
        return col;
    }

    bool letterBox(vec2 fragCoord, const vec2 resolution, const float aspect) { 
        if( fragCoord.x < 0. || fragCoord.x > resolution.x ||
            abs(2.*fragCoord.y-resolution.y) > resolution.x * (1./aspect) ) {
            return true;
        } else {
            return false;
        }
    }

    //
    // Cloud shape modelling and rendering 
    //
    float HenyeyGreenstein(float sundotrd, float g) 
    {
        float gg = g * g;
        return (1. - gg) / pow( 1. + gg - 2. * g * sundotrd, 1.5);
    }

    float interectCloudSphere( vec3 rd, float r ) 
    {
        float b = EARTH_RADIUS * rd.y;
        float d = b * b + r * r + 2. * EARTH_RADIUS * r;
        return -b + sqrt( d );
    }

    float linearstep( const float s, const float e, float v ) { return clamp( (v-s)*(1./(e-s)), 0., 1. ); }
    float linearstep0( const float e, float v ) { return min( v*(1./e), 1. ); }
    float remap(float v, float s, float e) { return (v - s) / (e - s); }

    float bufferB(vec2 fragCoord)
    {
        // pack 32x32x32 3d texture in 2d texture (with padding)
        float z = floor(fragCoord.x/34.) + 8.*floor(fragCoord.y/34.);
        vec2 uv = mod(fragCoord.xy, 34.) - 1.;
        vec3 coord = vec3(uv, z) / 32.;

        float r = tilableVoronoi( coord, 16,  3. );
        float g = tilableVoronoi( coord,  4,  8. );
        float b = tilableVoronoi( coord,  4, 16. );

        return max(0., 1.-(r + g * .5 + b * .25) / 1.75);
    }

    vec3 cloudShape(vec2 fragCoord)
    {
        vec2 vUV = fragCoord / canvassize.xy;
        vec3 coord = fract(vec3(vUV + vec2(.2,0.62), .5));
        
        vec3 col = vec3(1);
        
        float mfbm = 0.9;
        float mvor = 0.7;
        
        col.r = mix(1., tilableFbm( coord, 7, 4. ), mfbm) * 
            	mix(1., tilableVoronoi( coord, 8, 9. ), mvor);
        col.g = 0.625 * tilableVoronoi( coord + 0., 3, 15. ) +
        		0.250 * tilableVoronoi(  coord + 0., 3, 19. ) +
        		0.125 * tilableVoronoi( coord + 0., 3, 23. ) 
            	-1.;
        col.b = 1. - tilableVoronoi( coord + 0.5, 6, 9. );
        
	    return col;
    }

    float cloudMapBase(vec3 p, float norY) 
    {
        vec3 uv = p * (0.00005 * CLOUDS_BASE_SCALE);
        vec3 cloud = cloudShape(uv.xz);
    
        float n = norY*norY;
        n *= cloud.b ;
            n+= pow(1.-norY, 16.); 
        return remap( cloud.r - n, cloud.g, 1.);
    }

    float cloudMapDetail(vec3 p) 
    {
        // 3d lookup in 2d texture :(
        p = abs(p) * (0.0016 * CLOUDS_BASE_SCALE * CLOUDS_DETAIL_SCALE);
    
        float yi = mod(p.y,32.);
        ivec2 offset = ivec2(mod(yi,8.), mod(floor(yi/8.),4.))*34 + 1;
        float a = bufferB((mod(p.xz,32.)+vec2(offset.xy)+1.)/canvassize.xy).r;
        
        yi = mod(p.y+1.,32.);
        offset = ivec2(mod(yi,8.), mod(floor(yi/8.),4.))*34 + 1;
        float b = bufferB((mod(p.xz,32.)+vec2(offset.xy)+1.)/canvassize.xy).r;
        
        return mix(a,b,fract(p.y));
    }

    float cloudGradient( float norY ) { return linearstep( 0., .05, norY ) - linearstep( .8, 1.2, norY); }

    float cloudMap(vec3 pos, vec3 rd, float norY) {
        vec3 ps = pos;
        
        float m = cloudMapBase(ps, norY);
        m *= cloudGradient( norY );

        float dstrength = smoothstep(1., 0.5, m);
        
        // erode with detail
        if(dstrength > 0.) {
            m -= cloudMapDetail( ps ) * dstrength * CLOUDS_DETAIL_STRENGTH;
        }

        m = smoothstep( 0., CLOUDS_BASE_EDGE_SOFTNESS, m+(CLOUDS_COVERAGE-1.) );
        m *= linearstep0(CLOUDS_BOTTOM_SOFTNESS, norY);

        return clamp(m * CLOUDS_DENSITY * (1.+max((ps.x-7000.)*0.005,0.)), 0., 1.);
    }

    float volumetricShadow(in vec3 from, in float sundotrd ) {
        float dd = CLOUDS_SHADOW_MARGE_STEP_SIZE;
        vec3 rd = SUN_DIR;
        float d = dd * .5;
        float shadow = 1.0;

        for(int s=0; s<CLOUD_SELF_SHADOW_STEPS; s++) {
            vec3 pos = from + rd * d;
            float norY = (length(pos) - (EARTH_RADIUS + CLOUDS_BOTTOM)) * (1./(CLOUDS_TOP - CLOUDS_BOTTOM));

            if(norY > 1.) return shadow;

            float muE = cloudMap( pos, rd, norY );
            shadow *= exp(-muE * dd);

            dd *= CLOUDS_SHADOW_MARGE_STEP_MULTIPLY;
            d += dd;
        }
        return shadow;
    }

    vec4 renderClouds( vec3 ro, vec3 rd, inout float dist ) {
        if( rd.y < 0. ) {
            return vec4(0,0,0,10);
        }

        ro.xz *= SCENE_SCALE;
        ro.y = sqrt(EARTH_RADIUS*EARTH_RADIUS-dot(ro.xz,ro.xz));

        float start = interectCloudSphere( rd, CLOUDS_BOTTOM );
        float end  = interectCloudSphere( rd, CLOUDS_TOP );
        
        if (start > dist) {
            return vec4(0,0,0,10);
        }
        
        end = min(end, dist);
        
        float sundotrd = dot( rd, -SUN_DIR);

        // raymarch
        float d = start;
        float dD = (end-start) / float(CLOUD_MARCH_STEPS);

        float h = hash13(rd + fract(iTime) );
        d -= dD * h;

        float scattering =  mix( HenyeyGreenstein(sundotrd, CLOUDS_FORWARD_SCATTERING_G),
            HenyeyGreenstein(sundotrd, CLOUDS_BACKWARD_SCATTERING_G), CLOUDS_SCATTERING_LERP );

        float transmittance = 1.0;
        vec3 scatteredLight = vec3(0.0, 0.0, 0.0);

        dist = EARTH_RADIUS;

        for(int s=0; s<CLOUD_MARCH_STEPS; s++) {
            vec3 p = ro + d * rd;

            float norY = clamp( (length(p) - (EARTH_RADIUS + CLOUDS_BOTTOM)) * (1./(CLOUDS_TOP - CLOUDS_BOTTOM)), 0., 1.);

            float alpha = cloudMap( p, rd, norY );

            if(alpha > 0.)
            {
                dist = min( dist, d);
                vec3 ambientLight = mix( CLOUDS_AMBIENT_COLOR_BOTTOM, CLOUDS_AMBIENT_COLOR_TOP, norY );

                vec3 S = (ambientLight + SUN_COLOR * (scattering * volumetricShadow(p, sundotrd))) * alpha;
                float dTrans = exp(-alpha * dD);
                vec3 Sint = (S - S * dTrans) * (1. / alpha);
                scatteredLight += transmittance * Sint; 
                transmittance *= dTrans;
            }

            if( transmittance <= CLOUDS_MIN_TRANSMITTANCE ) break;

            d += dD;
        }

        return vec4(scatteredLight, transmittance);
    }

    //
    //
    // !Because I wanted a second cloud layer (below the horizon), I copy-pasted 
    // almost all of the code above:
    //
    float cloudMapLayer(vec3 pos, vec3 rd, float norY) {
        vec3 ps = pos;

        float m = cloudMapBase(ps, norY);
        // m *= cloudGradient( norY );
        float dstrength = smoothstep(1., 0.5, m);
        
        // erode with detail
        if (dstrength > 0.) {
            m -= cloudMapDetail( ps ) * dstrength * CLOUDS_DETAIL_STRENGTH;
        }

        m = smoothstep( 0., CLOUDS_BASE_EDGE_SOFTNESS, m+(CLOUDS_LAYER_COVERAGE-1.) );

        return clamp(m * CLOUDS_DENSITY, 0., 1.);
    }

    float volumetricShadowLayer(in vec3 from, in float sundotrd ) {
        float dd = CLOUDS_LAYER_SHADOW_MARGE_STEP_SIZE;
        vec3 rd = SUN_DIR;
        float d = dd * .5;
        float shadow = 1.0;

        for(int s=0; s<CLOUD_SELF_SHADOW_STEPS; s++) {
            vec3 pos = from + rd * d;
            float norY = clamp( (pos.y - CLOUDS_LAYER_BOTTOM ) * (1./(CLOUDS_LAYER_TOP - CLOUDS_LAYER_BOTTOM)), 0., 1.);

            if(norY > 1.) return shadow;

            float muE = cloudMapLayer( pos, rd, norY );
            shadow *= exp(-muE * dd);

            dd *= CLOUDS_SHADOW_MARGE_STEP_MULTIPLY;
            d += dd;
        }
        return shadow;
    }

    vec4 renderCloudLayer( vec3 ro, vec3 rd, inout float dist ) {
        if( rd.y > 0. ) {
            return vec4(0,0,0,10);
        }

        ro.xz *= SCENE_SCALE;
        ro.y = 0.;

        float start = CLOUDS_LAYER_TOP/rd.y;
        float end  = CLOUDS_LAYER_BOTTOM/rd.y;
        
        if (start > dist) {
            return vec4(0,0,0,10);
        }
        
        end = min(end, dist);
        
        float sundotrd = dot( rd, -SUN_DIR);

        // raymarch
        float d = start;
        float dD = (end-start) / float(CLOUD_MARCH_STEPS);

        float h = hash13(rd + fract(iTime) );
        d -= dD * h;

        float scattering =  mix( HenyeyGreenstein(sundotrd, CLOUDS_FORWARD_SCATTERING_G),
            HenyeyGreenstein(sundotrd, CLOUDS_BACKWARD_SCATTERING_G), CLOUDS_SCATTERING_LERP );

        float transmittance = 1.0;
        vec3 scatteredLight = vec3(0.0, 0.0, 0.0);

        dist = EARTH_RADIUS;

        for(int s=0; s<CLOUD_MARCH_STEPS; s++) {
            vec3 p = ro + d * rd;

            float norY = clamp( (p.y - CLOUDS_LAYER_BOTTOM ) * (1./(CLOUDS_LAYER_TOP - CLOUDS_LAYER_BOTTOM)), 0., 1.);

            float alpha = cloudMapLayer( p, rd, norY );

            if( alpha > 0. ) {
                dist = min( dist, d);
                vec3 ambientLight = mix( CLOUDS_AMBIENT_COLOR_BOTTOM, CLOUDS_AMBIENT_COLOR_TOP, norY );

                vec3 S = .7 * (ambientLight +  SUN_COLOR * (scattering * volumetricShadowLayer(p, sundotrd))) * alpha;
                float dTrans = exp(-alpha * dD);
                vec3 Sint = (S - S * dTrans) * (1. / alpha);
                scatteredLight += transmittance * Sint; 
                transmittance *= dTrans;
            }

            if( transmittance <= CLOUDS_MIN_TRANSMITTANCE ) break;

            d += dD;
        }

        return vec4(scatteredLight, transmittance);
    }

    void main()
    {        
        //if (fragCoord.y < 1.5) 
        //{
        //    fragColor = saveCamera(iTime, fragCoord, iMouse/canvassize.xyxy);
        //    if( abs(fragCoord.x-1.5)<0.5 ) fragColor = vec4(iMouse);
        //    if( abs(fragCoord.x-0.5)<0.5 ) fragColor = mouseChanged() ? vec4(0) : vec4(canvassize.xy,0,0);
        //} 
        //else 
        {
            if( letterBox(gl_FragCoord.xy, canvassize.xy, 2.25) ) {
                FragColor = vec4( 0., 0., 0., 1. );
                return;
            } else {
                float dist = 10000.0;// texelFetch(iChannel2, ivec2(gl_FragCoord),0).w * SCENE_SCALE;
                vec4 col = vec4(0,0,0,1);
                
                vec3 ro = vec3(0,0,0);
                vec3 rd = Texcoord;

                if( rd.y > 0. ) {
                    // clouds
                    col = renderClouds(ro, rd, dist);
                    float fogAmount = 1.-(.1 + exp(-dist*0.0001));
                    col.rgb = mix(col.rgb, getSkyColor(rd)*(1.-col.a), fogAmount);
                } else {
                    // cloud layer below horizon
                    col = renderCloudLayer(ro, rd, dist);
                    // height based fog, see https://iquilezles.org/articles/fog
                    float fogAmount = HEIGHT_BASED_FOG_C * 
                        (1.-exp( -dist*rd.y*(INV_SCENE_SCALE*HEIGHT_BASED_FOG_B)))/rd.y;
                    col.rgb = mix(col.rgb, getSkyColor(rd)*(1.-col.a), clamp(fogAmount,0.,1.));
                }

                if( col.w > 1. ) 
                {
                    FragColor = vec4(0,0,0,1);
                } 
                else 
                {
                    //vec2 spos = reprojectPos(ro+rd*dist, canvassize.xy, iChannel1);
                    //vec2 rpos = spos * canvassize.xy;

                    //if(!letterBox(rpos.xy, canvassize.xy, 2.3) && !resolutionChanged() && !mouseChanged()) 
                    //{
                    //    vec4 ocol = texture( iChannel1, spos, 0.0 ).xyzw;
                    //    col = mix(ocol, col, 0.05);
                    //}
                    FragColor = col;
                }
            }
        }
    }
);

static const std::string SkyboxHDRToCubeFragmentShader = GLSL(
    in vec3 Texcoord;
    out vec4 FragColor;
	uniform sampler2D hdrTexture;
	uniform int gradiant;
	uniform vec3 SunDirection;
	
	const vec2 invAtan = vec2(0.1591, 0.3183);
	vec2 SampleSphericalMap(vec3 v)
	{
		vec2 uv = vec2(atan(v.z, v.x), asin(-v.y));
		uv *= invAtan;
		uv += 0.5;
		return uv;
	}

	void main(void) 
	{
		if(gradiant > 0) 
		{ 
			float sun = clamp(dot(normalize(SunDirection * vec3(1,-1,1)), normalize(Texcoord)), 0.0, 1.0);
			vec3 col = mix(vec3(0.68,0.68,0.7), vec3(0.52941, 0.80784, 0.98039), Texcoord.y * 0.5 + 0.5);
			col += 0.5*vec3(1.0,0.5,0.1)*pow(sun, 20.0);
			FragColor = vec4(col - 0.3, 1.0);
		} 
		else
		{ 
			vec2 uv = SampleSphericalMap(normalize(Texcoord * vec3(-1, 1, -1))); // make sure to normalize localPos
			vec3 envColor = texture(hdrTexture, uv).rgb;            
			FragColor = vec4(envColor, 1.0f);
		} 

		//FragColor = vec4(1,0,0, 1.0f);
	}
);

static const std::string SkyboxIrradianceFragmentShader = GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	uniform samplerCube cubeMap;
	const float PI = 3.14159265359;
	const float TWO_PI = PI * 2;
	const float HALF_PI = PI * 0.5;
    const float sampleDelta = 0.025;
    const float totalSamples = (TWO_PI / sampleDelta) * (HALF_PI / sampleDelta); 
    const float invTotalSamples = 1.0 / totalSamples;


	void main(void) 
	{
        vec3 normal = normalize(Texcoord);
    
        vec3 irradiance = vec3(0.0);
        vec3 up    = vec3(0.0, 1.0, 0.0);
        vec3 right = normalize(cross(up, normal));
        up         = cross(normal, right);
    
        for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
        {
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
            {
                // spherical to cartesian (in tangent space)
                vec3 tangentSample = vec3(sin(theta) * cosPhi,  sin(theta) * sinPhi, cos(theta));
                // tangent space to world
                vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

                vec3 envColor = texture(cubeMap, sampleVec).rgb;
                envColor = envColor / (envColor + vec3(1.0));
                envColor = pow(envColor, vec3(1.0/2.2));
                irradiance += envColor * cos(theta) * sin(theta);
            }
        }

        irradiance *= PI * invTotalSamples;
        FragColor = vec4(irradiance, 1.0);
	}
);


static const std::string SkyboxBRDFVertexShader = GLSL(
	layout (location = 0) in vec3 vertexPosition;
	out vec2 TexCoord;
	
	void main()
	{
        TexCoord = (vertexPosition.xy + vec2(1.0)) / 2.0;
        gl_Position = vec4(vertexPosition, 1.0);
	}
);

static const std::string SkyboxBRDFFragmentShader = GLSL(
    in vec2 TexCoord;
    out vec4 FragColor;
    
    const float PI = 3.14159265359;
    float RadicalInverse_VdC(uint bits) 
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }
    
    vec2 Hammersley(uint i, uint N)
    {
        return vec2(float(i)/float(N), RadicalInverse_VdC(i));
    }
    
    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
    {
        float a = roughness*roughness;
        
        float phi = 2.0 * PI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        
        // from spherical coordinates to cartesian coordinates - halfway vector
        vec3 H;
        H.x = cos(phi) * sinTheta;
        H.y = sin(phi) * sinTheta;
        H.z = cosTheta;
        
        // from tangent-space H vector to world-space sample vector
        vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent   = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);
        
        vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
        return normalize(sampleVec);
    }
    
    float GeometrySchlickGGX(float NdotV, float roughness)
    {
        // note that we use a different k for IBL
        float a = roughness;
        float k = (a * a) / 2.0;
    
        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;
    
        return nom / denom;
    }
    
    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
        return ggx1 * ggx2;
    }
    
    vec2 IntegrateBRDF(float NdotV, float roughness)
    {
        vec3 V;
        V.x = sqrt(1.0 - NdotV*NdotV);
        V.y = 0.0;
        V.z = NdotV;
    
        float A = 0.0;
        float B = 0.0; 
    
        vec3 N = vec3(0.0, 0.0, 1.0);
        
        const uint SAMPLE_COUNT = 1024u;
        for(uint i = 0u; i < SAMPLE_COUNT; ++i)
        {
            // generates a sample vector that's biased towards the
            // preferred alignment direction (importance sampling).
            vec2 Xi = Hammersley(i, SAMPLE_COUNT);
            vec3 H = ImportanceSampleGGX(Xi, N, roughness);
            vec3 L = normalize(2.0 * dot(V, H) * H - V);
    
            float NdotL = max(L.z, 0.0);
            float NdotH = max(H.z, 0.0);
            float VdotH = max(dot(V, H), 0.0);
    
            if(NdotL > 0.0)
            {
                float G = GeometrySmith(N, V, L, roughness);
                float G_Vis = (G * VdotH) / (NdotH * NdotV);
                float Fc = pow(1.0 - VdotH, 5.0);
    
                A += (1.0 - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
        }
        A /= float(SAMPLE_COUNT);
        B /= float(SAMPLE_COUNT);
        return vec2(A, B);
    }
    

    void main() 
    {
        vec2 integratedBRDF = IntegrateBRDF(TexCoord.x, TexCoord.y);
        FragColor = vec4(integratedBRDF, 0, 1);
    }
);


static const std::string SkyboxPrefilterFragmentShader = GLSL(
	in vec3 Texcoord;
    out vec4 FragColor;
	
	uniform samplerCube cubeMap;
	uniform float roughness;
	
	const float PI = 3.14159265359;
    
	float DistributionGGX(vec3 N, vec3 H, float roughness)
	{
		float a = roughness*roughness;
		float a2 = a*a;
		float NdotH = max(dot(N, H), 0.0);
		float NdotH2 = NdotH*NdotH;
	
		float nom   = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;
	
		return nom / denom;
	}
    
	float RadicalInverse_VdC(uint bits) 
	{
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10; // / 0x100000000
	}
    
	vec2 Hammersley(uint i, uint N)
	{
		return vec2(float(i)/float(N), RadicalInverse_VdC(i));
	}
    
	vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
	{
		float a = roughness*roughness;
		
		float phi = 2.0 * PI * Xi.x;
		float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
		float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
		
		// from spherical coordinates to cartesian coordinates - halfway vector
		vec3 H;
		H.x = cos(phi) * sinTheta;
		H.y = sin(phi) * sinTheta;
		H.z = cosTheta;
		
		// from tangent-space H vector to world-space sample vector
		vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
		vec3 tangent   = normalize(cross(up, N));
		vec3 bitangent = cross(N, tangent);
		
		vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
		return normalize(sampleVec);
	}
    
	void main()
	{		
		vec3 N = normalize(Texcoord * vec3(1, 1, -1));
		
		// make the simplyfying assumption that V equals R equals the normal 
		vec3 R = N;
		vec3 V = R;
	
		const uint SAMPLE_COUNT = 1024u;
		vec3 prefilteredColor = vec3(0.0);
		float totalWeight = 0.0;
		
		for(uint i = 0u; i < SAMPLE_COUNT; ++i)
		{
			// generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
			vec2 Xi = Hammersley(i, SAMPLE_COUNT);
			vec3 H = ImportanceSampleGGX(Xi, N, roughness);
			vec3 L  = normalize(2.0 * dot(V, H) * H - V);
	
			float NdotL = max(dot(N, L), 0.0);
			if(NdotL > 0.0)
			{
				// sample from the environment's mip level based on roughness/pdf
				float D   = DistributionGGX(N, H, roughness);
				float NdotH = max(dot(N, H), 0.0);
				float HdotV = max(dot(H, V), 0.0);
				float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 
	
				float resolution = 512.0; // resolution of source cubemap (per face)
				float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
				float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
	
				float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
			
				prefilteredColor += textureLod(cubeMap, L, mipLevel).rgb * NdotL;
				totalWeight      += NdotL;
			}
		}
	
		prefilteredColor = prefilteredColor / totalWeight;
        prefilteredColor = prefilteredColor / (prefilteredColor + vec3(1.0));
        prefilteredColor = pow(prefilteredColor, vec3(1.0/2.2));
        
		FragColor = vec4(prefilteredColor, 1.0);
        //FragColor = vec4(1,0,0,1);
        //FragColor = texture(cubeMap, Texcoord);
	}
);