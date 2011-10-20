/*
 * IndexedDataFileExceptions.h
 *
 *  Created on: 13/10/2011
 *      Author: fernando
 */

#ifndef INDEXEDDATAFILEEXCEPTIONS_H_
#define INDEXEDDATAFILEEXCEPTIONS_H_

class IndexedDataException:public std::exception{};
class IndexedDataRecordNotFoundException:public IndexedDataException{};
class IndexedDataNoMoreRecordsInBlockException:public IndexedDataException{};
class IndexedDataNextException : public IndexedDataException{};
class IndexedDataInsertException : public IndexedDataException{};
#endif /* INDEXEDDATAFILEEXCEPTIONS_H_ */
