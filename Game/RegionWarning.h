#pragma once

typedef enum RegionWarning {
	REGIONWARNING_JAPAN,
	REGIONWARNING_SKIP,
	REGIONWARNING_KOREA,
	REGIONWARNING_REGIONSETTING
} RegionWarning;
extern RegionWarning CurrentRegionWarning;

