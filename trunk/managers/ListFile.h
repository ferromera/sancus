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

class ListFile {
	enum lastSearchEnum {ELECTION_SEARCH,NAME_SEARCH,PRIMARY_SEARCH,NO_SEARCH};
	BPlusTree<SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>,4096> * electionIndex;
	BPlusTree<SecondaryIndexRecord<StringKey,ListRecord::Key>,4096> * nameIndex;

	BPlusTree<PrimaryIndexRecord<ListRecord::Key>,4096> * primaryIndex;
	IndexedDataFile<ListRecord,8192> * dataFile;
	enum lastSearchEnum lastSearch;

	ElectionRecord::Key * electionSearched;
	StringKey * nameSearched;
	ListRecord * found;


	static ListFile * instance;

public:

	static ListFile * getInstance();

	ListFile();

	void insert(const ListRecord &);
	void remove(const ListRecord &);
	void update(const ListRecord &);
	const ListRecord & searchByElection(const ElectionRecord::Key & election);
	const ListRecord & searchByName(const std::string & name);
	const ListRecord & search(const ListRecord::Key & list);
	const ListRecord & VoteCountingFile::nextElection();
	const ListRecord & VoteCountingFile::nextName();
	const ListRecord & VoteCountingFile::next();

	virtual ~ListFile();
};

#endif /* LISTFILE_H_ */


