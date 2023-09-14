#pragma once
#include "AbstractScene.h"
#include <vector>
#include <string>

class Guide
{
    
public:
    Guide() = default;
    ~Guide() = default;
    void DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const;
    void DrawGuideElement(const guideElement& guide, float& currentX, float y) const;
    
};
