#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {

	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	ResetMapChipData();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv;

	mapChipCsv << file.rdbuf();

	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {


	// X番号
	uint32_t xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);

	// 反転前Y番号
	uint32_t beforeYindex = static_cast<uint32_t>((position.y + kBlockHeight / 2.0f) / kBlockHeight);

	// 正しいY番号
	uint32_t yIndex = kNumBlockVirtical - 1 - beforeYindex;

	IndexSet indexSet{xIndex, yIndex};

	return indexSet;
}

mapChipRect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) { 
	
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	mapChipRect rect;
	rect.left = center.x - kBlockWidth / 2.0f;    
	rect.right = center.x + kBlockWidth / 2.0f;  
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}
