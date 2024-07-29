#pragma once
#include <vector>
#include "Vector3.h"
#include <string>


	enum class MapChipType {
		kBlank,
		kBlock,
	};

	struct MapChipData {
		std::vector<std::vector<MapChipType>> data;
	};

	struct IndexSet {
	    uint32_t xIndex;
	    uint32_t yIndex;
	};

	struct mapChipRect {
	    float left;
	    float right;
	    float bottom;
	    float top;
	};

class MapChipField {

	public:

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	mapChipRect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);
};
