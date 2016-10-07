
//#include "TEveStraightLineSet.h"
#include "TEveBox.h"
#include "TEveLine.h"
#include "TEveCompound.h"
#include "TEveGeoNode.h"

#include "Fireworks/Core/interface/FWProxyBuilderBase.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "Fireworks/Core/interface/fwLog.h"

#include "DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"

class FWME0DigiProxyBuilder : public FWProxyBuilderBase
{
public:
  FWME0DigiProxyBuilder() {}
  virtual ~FWME0DigiProxyBuilder() {}

  REGISTER_PROXYBUILDER_METHODS();

private:
  virtual void build(const FWEventItem* iItem, TEveElementList* product, const FWViewContext*);
  FWME0DigiProxyBuilder(const FWME0DigiProxyBuilder&);    
  const FWME0DigiProxyBuilder& operator=(const FWME0DigiProxyBuilder&);
};

void FWME0DigiProxyBuilder::build(const FWEventItem* iItem, TEveElementList* product, const FWViewContext*)
{
    const ME0DigiPreRecoCollection* digis = 0;
    iItem->get(digis);

    if ( ! digis ) 
    {
        std::cerr  << "Failed to get ME0Digis" <<std::endl;
        return;
    }
    const FWGeometry *geom = iItem->getGeom();

    int topCnt = 0;
    for ( ME0DigiPreRecoCollection::DigiRangeIterator dri = digis->begin(), driEnd = digis->end();
          dri != driEnd; ++dri )
    {
        unsigned int rawid = (*dri).first.rawId();
        const ME0DigiPreRecoCollection::Range& range = (*dri).second;

        if( ! geom->contains( rawid ))
        {
            std::cerr << "Failed to get geometry of ME0 roll with detid: "
                      << rawid << std::endl;
      
      
            continue;
        }
          
        int cntDS = 0;
        for( ME0DigiPreRecoCollection::const_iterator dit = range.first;
             dit != range.second; ++dit )
        {
            cntDS++;
        }
          
        for( ME0DigiPreRecoCollection::const_iterator dit = range.first;
             dit != range.second; ++dit )
        {     
            float localPoint[3] =    {(*dit).x(), (*dit).y(), 0.0};
            float globalPoint[3];
            geom->localToGlobal( rawid, localPoint, globalPoint);
            
            float x = globalPoint[0];
            float y = globalPoint[1];
            float z = globalPoint[2];
            
            float dx = (*dit).ex();
            float dy = (*dit).ey();
            float dz = 0.1;
            
            TEveBox* box = new TEveBox(); 
            // points are set clockwise
            box->SetVertex(0, x - dx, y + dy, z - dz );
            box->SetVertex(1, x + dx, y + dy, z - dz );
            box->SetVertex(2, x + dx, y - dy, z - dz );
            box->SetVertex(3, x - dx, y - dy, z - dz );

            box->SetVertex(4, x - dx, y + dy, z + dz );
            box->SetVertex(5, x + dx, y + dy, z + dz );
            box->SetVertex(6, x + dx, y - dy, z + dz );
            box->SetVertex(7, x - dx, y - dy, z + dz );
            
            setupAddElement( box, product );
        }

        topCnt++;          
    }
}


REGISTER_FWPROXYBUILDER(FWME0DigiProxyBuilder, ME0DigiPreRecoCollection, "ME0Digi", 
                        FWViewType::kAll3DBits | FWViewType::kAllRPZBits);
