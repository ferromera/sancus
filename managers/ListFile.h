/*
 * ListFile.h
 *
 *  Created on: 16/10/2011
 *      Author: fernando
 */

#ifndef LISTFILE_H_
#define LISTFILE_H_

#define LIST_FILE_NAME_INDEX_PATH "./ListFileNameIndex.bin"
#define LIST_FILE_ELECTION_INDEX_PATH "./ListFileElectionIndex.bin"
#define LIST_FILE_PRIMARY_INDEX_PATH "./ListFilePrimaryIndex.bin"
#define LIST_FILE_DATA_PATH	"./ListFileData.bin"

#include "FileManagerExceptions.h"
#include "../estructuras/BPlusTree.h"
#include "../estructuras/IndexedDataFile.h"
#include "../records/SecondaryIndexRecord.h"
#include "../records/PrimaryIndexRecord.h"
#include "../records/ListRecord.h"

#define LIST_FILE__SEC_INDEX_BLOCK_SIZE 1024
#define LIST_FILE__PRI_INDEX_BLOCK_SIZE 512
#define LIST_FILE_DATA_BLOCK_SIZE 1024

class ListFile {
	enum lastSearchEnum {
		ELECTION_SEARCH, NAME_SEARCH, PRIMARY_SEARCH, NO_SEARCH
	};
	BPlusTree<SecondaryIndexRecord<ElectionRecord::Key, ListRecord::Key> , LIST_FILE__SEC_INDEX_BLOCK_SIZE>
			* electionIndex;
	BPlusTree<SecondaryIndexRecord<StringKey, ListRecord::Key> , LIST_FILE__SEC_INDEX_BLOCK_SIZE> * nameIndex;

	BPlusTree<PrimaryIndexRecord<ListRecord::Key> , LIST_FILE__PRI_INDEX_BLOCK_SIZE> * primaryIndex;
	IndexedDataFile<ListRecord, LIST_FILE_DATA_BLOCK_SIZE> * dataFile;
	enum lastSearchEnum lastSearch;

	ElectionRecord::Key * electionSearched;
	StringKey * nameSearched;
	ListRecord * found;

	static ListFile * instance;

public:

	static ListFile * getInstance();
	static void deleteInstance() {
		delete instance;
		instance = NULL;
	}

	ListFile();

	void insert(const ListRecord &);
	void remove(const ListRecord &);
	void update(const ListRecord &);
	const ListRecord & searchByElection(const ElectionRecord::Key & election);
	const ListRecord & searchByName(const std::string & name);
	const ListRecord & search(const ListRecord::Key & list);
	const ListRecord & nextElection();
	const ListRecord & nextName();
	const ListRecord & next();
	void report();
	void createReportFile();

	virtual ~ListFile();
};

#endif /* LISTFILE_H_ */

