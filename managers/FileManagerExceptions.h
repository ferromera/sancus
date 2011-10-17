/*
 * FileManagerExceptions.h
 *
 *  Created on: 17/10/2011
 *      Author: fernando
 */

#ifndef FILEMANAGEREXCEPTIONS_H_
#define FILEMANAGEREXCEPTIONS_H_

class FileManagerException: public std::exception{};
class FileInsertException: public FileManagerException{};
class FileRemoveException: public FileManagerException{};
class FileUpdateException: public FileManagerException{};
class FileSearchException: public FileManagerException{};
class FileNextException: public FileManagerException{};
#endif /* FILEMANAGEREXCEPTIONS_H_ */
