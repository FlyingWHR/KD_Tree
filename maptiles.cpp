/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */
	 			
#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles)
{
    /**
     * @todo Implement this function!
     */
    int r=theSource.getRows();
    int c=theSource.getColumns();
    MosaicCanvas * Mosaic=new MosaicCanvas(r,c);
    vector<Point<3>> points;
	map<Point<3>, int> tmap;
	for (size_t i=0;i<theTiles.size();i++){
		RGBAPixel t=theTiles[i].getAverageColor();
		Point<3> tile(t.red,t.green,t.blue);
		points.push_back(tile);
		tmap.insert (pair<Point<3>,int>(tile,i));
	}

	KDTree<3> KDT(points);
	for (int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			RGBAPixel p=theSource.getRegionColor(i,j);
			Point<3>org_tile(p.red,p.green,p.blue);
			Mosaic->setTile(i,j,theTiles[tmap[KDT.findNearestNeighbor(org_tile)]]);
		}
	} 
	return Mosaic;
}
