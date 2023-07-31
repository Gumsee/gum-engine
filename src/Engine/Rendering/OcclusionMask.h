#pragma once

class OcclusionMask
{
private:
	//MaskedOcclusionCulling *pMoc;
	float nearClipDist = 1.0f;

public:
    OcclusionMask(int width, int height);
    ~OcclusionMask();

    void render();
    void clean();
};
