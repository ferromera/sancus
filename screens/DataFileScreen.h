/*
 * DataFileScreen.h
 *
 *  Created on: 20/11/2011
 *      Author: juanmromera
 */

#ifndef DATAFILESCREEN_H_
#define DATAFILESCREEN_H_
#include "Drawable.h"

class DataFileScreen : public Drawable{
public:
	DataFileScreen();
	void draw();
	void createReportFileByDistrict();
	void createReportFileByVoter();
	void createReportFileByElection();
	void createReportFileByList();
	void createReportFileByCandidate();
	void createReportFileByCharge();
	void createReportFileByCounting();
	void createReportFileByAdministrator();
};

#endif /* DATAFILESCREEN_H_ */
