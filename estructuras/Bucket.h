/*
 * Area de almacenamiento donde se persisten elementos que tienen en común alguna propiedad.
 *
 * @author Alfredo Scoppa
 * @since Sep 25, 2011
 */

#ifndef BUCKET_H_
#define BUCKET_H_

#include "stdint.h"

#define BUCKET_SPACE bucketSize-3

#pragma pack(push)
#pragma pack(1)
template<unsigned int bucketSize>
struct Bucket{
	uint8_t overflow;
	uint16_t freeSpace;
	char bytes[BUCKET_SPACE];
};

#pragma pack(pop)

#endif /* BUCKET_H_ */
