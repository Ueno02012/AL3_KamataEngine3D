#pragma once
#include <vector>
#include <map>
#include <string>
#include <Vector3.h>
enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

namespace {
	std::map<std::string, MapChipType> mapChipTable = {
		{"0",MapChipType::kBlank},
		{"1",MapChipType::kBlock},
	};

}

class MapChipField {

public:
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	int GetNumBlockVirtical() { return kNumBlockVirtical; };
	int GetNumBlockHorizontal() { return kNumBlockHorizontal; };

private:

	MapChipData mapChipData_;

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;




};
