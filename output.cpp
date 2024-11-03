#include "output.h"

Output::Output()
{
    for(int i=0;i<_outputSetup._numberOfLines;i++)
    {
        line l;
        lines.push_back(l);
    }

    for(auto &line:lines)
    {
        for(int i=0;i<_outputSetup._totalCupNumber;i++)
        {
            cup _cup;
            line.cups.push_back(_cup);
        }

        for(auto &cup:line.cups)
        {
            for(int i=0;i<_outputSetup._batchSize / 2;i++)
            {
                image _img;
                cup.Images.push_back(_img);
            }

            for(auto &image:cup.Images)
            {
                image.detectedObjects.resize(_outputSetup._numberOfClasses);
                image.detectedObjects.fill(false);
                image.numberOfDefects.resize(_outputSetup._numberOfClasses);
                image.numberOfDefects.fill(0);
                image.colorPercents.resize(_outputSetup.numberOfColors);
                image.colorPercents.fill(0);
            }
        }
    }

}
