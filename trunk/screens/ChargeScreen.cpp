/*
 * ChargeScreen.cpp
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#include "ChargeScreen.h"
#include "../core/VoteApp.h"
#include "../managers/DistrictFile.h"
#include "../managers/FileManagerExceptions.h"
//#include "../records/DistrictRecord.h"
#include "../managers/VoterFile.h"
#include "../records/VoterRecord.h"
#include "../records/ElectionRecord.h"
#include "../records/CandidateRecord.h"
#include "../records/VoteCountingRecord.h"
#include "../records/ChargeRecord.h"
#include "../managers/ElectionFile.h"
#include "../managers/ChargeFile.h"
#include "../managers/AdministratorFile.h"
#include "../records/AdministratorRecord.h"
#include <cstdio>

ChargeScreen::ChargeScreen() {
}
void ChargeScreen::draw() {
	int opcion;
	VoteApp* app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE ALTA" << std::endl;
	std::cout << "Eliga el archivo donde quiera ingresar un alta y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Archivo de Distritos" << std::endl;
	std::cout << "2- Archivo de Votantes" << std::endl;
	std::cout << "3- Archivo de Elecciones" << std::endl;
	std::cout << "4- Archivo de Candidatos" << std::endl;
	std::cout << "5- Archivo de Listas" << std::endl;
	std::cout << "6- Archivo de Cargos" << std::endl;
	std::cout << "7- Archivo de Conteo" << std::endl;
	std::cout << "8- Archivo de Administrador" << std::endl;
	std::cout << "9- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			while (chargeDistrict() == 'S') {
			}
			;
			return;
		case 2:
			while (chargeVoter() == 'S') {
			}
			;
			return;
		case 3:
			while (chargeElection() == 'S') {
			}
			;
			return;
		case 4:
			while (chargeCandidate() == 'S') {
			}
			;
			return;
		case 5:
			while (chargeList() == 'S') {
			}
			;
			return;
		case 6:
			while (chargeCharge() == 'S') {
			}
			;
			return;
		case 7:
			while (chargeCounting() == 'S') {
			}
			;
			return;
		case 8:
			while (chargeAdministrator() == 'S') {
			}
			;
			return;
		case 9:
			app->setActualScreen(ADM_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, eliga nuevamente" << std::endl;
		}
	}
}
char ChargeScreen::chargeDistrict() {
	std::string district;
	std::string father;
	system("clear");

	std::cout << "Alta de un registro del Archivo de Distritos" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Nombre de un Distrito y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	district = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout
			<< "Ingrese el Nombre del padre del Distrito anterior, si no tiene padre no ingrese nada y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	father = IstreamUtils::getString();
	std::cout << "" << std::endl;

	DistrictFile* dFile = DistrictFile::getInstance();
	DistrictRecord::Key keyFather;
	DistrictRecord record = DistrictRecord(district);
	if (father.compare("") != 0) {
		keyFather = DistrictRecord::Key(father);
		try {
			DistrictRecord dRecordReturned = dFile->search(keyFather);
			if (dRecordReturned.getKey() != keyFather)
				throw FileSearchException();
		} catch (FileSearchException e) {
			std::cout << "No se pude Insertar el Registro por que no existe el Distrito padre elegido" << std::endl;
			std::cout << "" << std::endl;
			std::cout << "Quiere agregar otro registro al archivo de Distritos S/N" << std::endl;
			dFile->report();
			return doQuestion();
		}
		record = DistrictRecord(district, father);
	}
	try {
		dFile->insert(record);
	} catch (FileInsertException e) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Distritos S/N" << std::endl;
		dFile->report();
		return doQuestion();
	}
	std::cout << "Se inserto correctamente el siguiente Registo distrito: ";
	std::cout << record.getKey().getString() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Distritos S/N" << std::endl;
	dFile->report();
	return doQuestion();
}
char ChargeScreen::chargeVoter() {
	uint32_t dni;
	std::string name;
	std::string distrito;
	std::string domicilio;
	std::string clave;
	system("clear");
	std::cout << "Alta de un registro del Archivo de Votantes" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el DNI y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	dni = IstreamUtils::getUint();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Nombre y Apellido presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	name = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Distrito y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Domicilio y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	domicilio = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese la Contraseña y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	clave = IstreamUtils::getString();
	std::cout << "" << std::endl;

	// Busco si existe el Distrito
	std::list<ElectionRecord::Key> listaDeElecciones;
	DistrictRecord::Key dRecordKey = DistrictRecord::Key(distrito);

	DistrictFile* dFile = DistrictFile::getInstance();
	try {
		DistrictRecord record = dFile->search(dRecordKey);
	} catch (FileSearchException &e) {
		std::cout << "Error el Distrito elegido no existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Votantes S/N" << std::endl;
		return doQuestion();
	}

	VoterRecord record = VoterRecord(name, dni, domicilio, clave, dRecordKey, listaDeElecciones);
	VoterFile* vFile = VoterFile::getInstance();
	try {
		vFile->insert(record);
	} catch (FileInsertException e) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Votantes S/N" << std::endl;
		return doQuestion();
	}

	std::cout << "Se inserto correctamente el siguiente Registo votante: ";
	std::cout << record.getKey().getUint() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Votantes S/N" << std::endl;
	return doQuestion();
}
char ChargeScreen::chargeElection() {
	int fecha;
	std::string cargo;
	std::string distrito;
	system("clear");
	std::cout << "Alta de un registro del Archivo de Elecciones" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese la fecha de la Eleccion con el siguiente formato YYYYMMDD y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	bool fechaIncorrecta = true;
	while (fechaIncorrecta) {
		fecha = IstreamUtils::getUint();
		std::cout << "" << std::endl;
		if (fecha < Time::getCurrentDate()) {
			std::cout << "La fecha es menor a la actual ingrese una fecha posterior al " << Time::getCurrentDate()
					<< std::endl;
			std::cout << "" << std::endl;
		} else
			fechaIncorrecta = false;
	}

	std::cout << "Ingrese el Nombre del Cargo a votarse en la eleccion y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	cargo = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Distrito donde se elegira al Cargo anteriormente seleccionado y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;

	ChargeRecord::Key chargeKey = ChargeRecord::Key(cargo, distrito);
	ChargeFile* cFile = ChargeFile::getInstance();
	ChargeRecord cRecord;
	try {
		cRecord = cFile->search(chargeKey);
		if (cRecord.getKey() != chargeKey)
			throw FileSearchException();
	} catch (FileSearchException e) {
		std::cout << "Error el Cargo " << cargo << " con el distrito " << distrito
				<< " no existe en el archivo de Cargos " << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Elecciones S/N" << std::endl;
		return doQuestion();
	}

	ElectionFile* eFile = ElectionFile::getInstance();
	ElectionRecord record = ElectionRecord(fecha, cRecord.getKey(), cRecord.getDistrict());
	try {
		eFile->insert(record);
	} catch (FileInsertException) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Elecciones S/N" << std::endl;
		return doQuestion();
	}

	std::cout << "Se inserto correctamente el siguiente Registo eleccion: ";
	std::cout << record.getKey().getDate() << "/" << record.getKey().getCharge().getCharge() << "/"
			<< record.getKey().getCharge().getDistrict().getString() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Elecciones S/N" << std::endl;
	return doQuestion();

}
char ChargeScreen::chargeList() {
	int fecha;
	std::string name;
	std::string cargo;
	std::string distrito;
	system("clear");
	std::cout << "Alta de un registro del Archivo de Listas" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el nombre de la lista y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	name = IstreamUtils::getString();
	std::cout
			<< "Ingrese la fecha de la Eleccion a la que la Lista se presentara con el siguiente formato YYYYMMDD y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	bool fechaIncorrecta = true;
	while (fechaIncorrecta) {
		fecha = IstreamUtils::getUint();
		std::cout << "" << std::endl;
		if (fecha < Time::getCurrentDate()) {
			std::cout << "La fecha es menor a la actual ingrese una fecha posterior al " << Time::getCurrentDate()
					<< std::endl;
			std::cout << "" << std::endl;
		} else
			fechaIncorrecta = false;
	}
	std::cout
			<< "Ingrese el Nombre del Cargo a votarse en la eleccion a la cual la Lista se presentara y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	cargo = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Distrito donde se elegira al Cargo anteriormente seleccionado y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;

	ElectionFile * eFile = ElectionFile::getInstance();
	ChargeRecord::Key chargeKey(cargo, distrito);
	ElectionRecord elRecord;

	ElectionRecord::Key electionKey = ElectionRecord::Key(fecha, chargeKey);
	try {
		elRecord = eFile->search(electionKey);
		if (elRecord.getKey() != electionKey)
			throw FileSearchException();
	} catch (FileSearchException e) {
		std::cout << "Error la eleccion  " << electionKey.getString() << " no existe en el archivo de Elecciones "
				<< std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Listas S/N" << std::endl;
		return doQuestion();
	}

	ListFile* lFile = ListFile::getInstance();
	ListRecord record = ListRecord(elRecord.getKey(), name);
	try {
		lFile->insert(record);
	} catch (FileInsertException e) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Listas S/N" << std::endl;
		return doQuestion();
	}

	std::cout << "Se inserto correctamente el siguiente Registo lista: ";
	std::cout << record.getKey().getString() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Listas S/N" << std::endl;
	return doQuestion();

}
char ChargeScreen::chargeCandidate() {
	uint32_t dni;
	int fecha;
	std::string name;
	std::string cargo;
	std::string candCargo;
	std::string distrito;
	system("clear");
	std::cout << "Alta de un registro del Archivo de Candidatos" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el DNI del candidato y presione ENTER" << std::endl;
	bool candidatoIncorrecto = true;
	while (candidatoIncorrecto) {
		dni = IstreamUtils::getUint();
		std::cout << "" << std::endl;
		if (!validateDNI(dni)) {
			std::cout << "La persona ingresada no existe en el sistema, ingrese otro DNI" << std::endl;
			std::cout << "" << std::endl;
		} else
			candidatoIncorrecto = false;
	}
	std::cout << "" << std::endl;
	std::cout << "Ingrese el nombre de la lista en la cual figurara el candidato y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	name = IstreamUtils::getString();
	std::cout
			<< "Ingrese la fecha de la Eleccion a la que la Lista se presentara con el siguiente formato YYYYMMDD y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	bool fechaIncorrecta = true;
	while (fechaIncorrecta) {
		fecha = IstreamUtils::getUint();
		std::cout << "" << std::endl;
		if (fecha < Time::getCurrentDate()) {
			std::cout << "La fecha es menor a la actual ingrese una fecha posterior al " << Time::getCurrentDate()
					<< std::endl;
			std::cout << "" << std::endl;
		} else
			fechaIncorrecta = false;
	}
	std::cout
			<<"Ingrese el nombre del cargo principal de la eleccion a la cual la Lista se presentara y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	cargo = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Distrito donde se elegira al Cargo anteriormente seleccionado y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout
			<<"Ingrese el nombre del cargo del candidato y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	candCargo = IstreamUtils::getString();
	std::cout << "" << std::endl;

	ListFile* lFile = ListFile::getInstance();
	ChargeRecord::Key chargeKey(cargo, distrito);
	ListRecord listRecord;

	ElectionRecord::Key electionKey = ElectionRecord::Key(fecha, chargeKey);
	ListRecord::Key listaKey = ListRecord::Key(electionKey, name);
	try {
		listRecord = lFile->search(listaKey);
		if (listRecord.getKey() != listaKey)
			throw FileSearchException();
	} catch (FileSearchException e) {
		std::cout << "Error la lista  " << listaKey.getString() << " no existe en el archivo de Listas " << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Candidato S/N" << std::endl;
		return doQuestion();
	}

	ChargeRecord::Key charge2Key = ChargeRecord::Key(candCargo, distrito);
	ChargeFile* chFile = ChargeFile::getInstance();
	ChargeRecord cRecord;
	try {
		cRecord = chFile->search(charge2Key);
		if (cRecord.getKey() != charge2Key)
			throw FileSearchException();
	} catch (FileSearchException e) {
		std::cout << "Error el Cargo " << cargo << " con el distrito " << distrito
				<< " no existe en el archivo de Cargos " << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Elecciones S/N" << std::endl;
		return doQuestion();
	}

	CandidateFile* cFile = CandidateFile::getInstance();
	CandidateRecord record = CandidateRecord(listaKey, cRecord.getKey() , user.getKey());
	try {
		cFile->insert(record);
	} catch (FileInsertException) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Listas S/N" << std::endl;
		return doQuestion();
	}

	std::cout << "Se inserto correctamente el siguiente Registo candidato: ";
	std::cout << record.getKey().getString() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Candidatos S/N" << std::endl;
	return doQuestion();
}
char ChargeScreen::chargeCharge() {
	std::string charge;
	std::string distrito;
	std::string father;
	system("clear");

	std::cout << "Alta de un registro del Archivo de Cargos" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Nombre de un Cargo y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	charge = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Nombre del distrito donde se eligira ese cargo y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Nombre del padre del Cargo anterior, si no tiene padre no ingrese nada y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	father = IstreamUtils::getString();
	std::cout << "" << std::endl;

	DistrictFile* dFile = DistrictFile::getInstance();
	ChargeFile* cFile = ChargeFile::getInstance();
	ChargeRecord::Key chargeKeyFather;
	DistrictRecord dRecord;
	DistrictRecord::Key dKey = DistrictRecord::Key(distrito);
	ChargeRecord chargeFatherRecord;
	ChargeRecord record;
	try {
		dRecord = dFile->search(dKey);
		if (dRecord.getKey() != dKey)
			throw FileSearchException();
	} catch (FileSearchException e) {
		std::cout << "Error el distrito ingresado es inexistente" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Cargos S/N" << std::endl;
		return doQuestion();
	}

	if (father.compare("") != 0) {
		chargeKeyFather = ChargeRecord::Key(father, dRecord.getKey());
		try {
			chargeFatherRecord = cFile->search(chargeKeyFather);
			if (chargeFatherRecord.getKey() != chargeKeyFather)
				throw FileSearchException();
		} catch (FileSearchException e) {
			std::cout << "Error el cargo padre ingresado es inexistente" << std::endl;
			std::cout << "" << std::endl;
			std::cout << "Quiere agregar otro registro al archivo de Cargos S/N" << std::endl;
			return doQuestion();
		}
		record = ChargeRecord(charge, dRecord.getKey(), chargeFatherRecord.getChargeName(),
				chargeFatherRecord.getDistrict());
	} else {
		record = ChargeRecord(charge, distrito);
	}

	try {
		cFile->insert(record);
	} catch (FileInsertException) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Cargos S/N" << std::endl;
		return doQuestion();
	}
	std::cout << "Se inserto correctamente el siguiente Registo cargo: ";
	std::cout << record.getKey().getString() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Distritos S/N" << std::endl;
	return doQuestion();
}
char ChargeScreen::chargeCounting() {
	int fecha;
	std::string name;
	std::string cargo;
	std::string distrito;
	std::string distritoVotante;

	system("clear");
	std::cout << "Alta de un registro del Archivo de Conteo" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el nombre de la Lista y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	name = IstreamUtils::getString();
	std::cout
			<< "Ingrese la fecha de la Eleccion a la que la Lista se presentara con el siguiente formato YYYYMMDD y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	bool fechaIncorrecta = true;
	while (fechaIncorrecta) {
		fecha = IstreamUtils::getUint();
		std::cout << "" << std::endl;
		if (fecha < Time::getCurrentDate()) {
			std::cout << "La fecha es menor a la actual ingrese una fecha posterior al " << Time::getCurrentDate()
					<< std::endl;
			std::cout << "" << std::endl;
		} else
			fechaIncorrecta = false;
	}
	std::cout
			<< "Ingrese el Nombre del Cargo a votarse en la eleccion a la cual la Lista se presentara y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	cargo = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout << "Ingrese el Distrito donde se elegira al Cargo anteriormente seleccionado y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	distrito = IstreamUtils::getString();
	std::cout << "" << std::endl;
	std::cout
			<< "Ingrese el Distrito del votante donde se elegira al Cargo anteriormente seleccionado y presione ENTER"
			<< std::endl;
	std::cout << "" << std::endl;
	distritoVotante = IstreamUtils::getString();
	std::cout << "" << std::endl;

	 ElectionFile * eFile = ElectionFile::getInstance();
	 ChargeRecord::Key chargeKey(cargo,distrito);
	 ElectionRecord::Key electionKey = ElectionRecord::Key(fecha,chargeKey);

	 try
	 {
		 ElectionRecord elRecord = eFile->search(electionKey);
		 if(elRecord.getKey() != electionKey)
			 throw FileSearchException();
	 }catch(FileSearchException e)
	 {
	 std::cout<<"Error la eleccion  "<<electionKey.getString()<<" no existe en el archivo de Elecciones "<<std::endl;
	 std::cout<<""<<std::endl;
	 std::cout<<"Quiere agregar otro registro al archivo de Conteo S/N"<<std::endl;
	 return doQuestion();
	 }

	 ListFile* lFile = ListFile::getInstance();
	 ListRecord::Key listKey = ListRecord::Key(electionKey,name);

	 try
	 {
		 ListRecord listRecord = lFile->search(listKey);
		 if(listRecord.getKey() != listKey)
			 throw FileSearchException();
	 }catch(FileSearchException e)
	 {
		 std::cout<<"Error la lista  "<<listKey.getString()<<" no existe en el archivo de Listas "<<std::endl;
		 std::cout<<""<<std::endl;
		 std::cout<<"Quiere agregar otro registro al archivo de Conteo S/N"<<std::endl;
		 return doQuestion();
	 }
	 DistrictFile* dFile = DistrictFile::getInstance();
	 DistrictRecord::Key dKey = DistrictRecord::Key(distritoVotante);
	 DistrictRecord districtRecord;
	 try
	 {
		districtRecord = dFile->search(dKey);
		if(districtRecord.getKey() != dKey)
			throw FileSearchException();
	 }catch(FileSearchException e)
	 {
	 std::cout<<"Error el distrito del votante  "<<listKey.getString()<<" no existe en el archivo de Distrito "<<std::endl;
	 std::cout<<""<<std::endl;
	 std::cout<<"Quiere agregar otro registro al archivo de Conteo S/N"<<std::endl;
	 return doQuestion();
	 }

	 VoteCountingFile* vcFile = VoteCountingFile::getInstance();
	 VoteCountingRecord record = VoteCountingRecord(listKey,districtRecord.getKey(),electionKey,0);
	 try{
		 vcFile->insert(record);
	 }catch(FileInsertException e)
	 {
	 std::cout<<"Error el Registro ya existe"<<std::endl;
	 std::cout<<""<<std::endl;
	 std::cout<<"Quiere agregar otro registro al archivo de Listas S/N"<<std::endl;
	 return doQuestion();
	 }

	 std::cout<<"Se inserto correctamente el siguiente Registo Conteo: ";
	 std::cout<<record.getKey().getString()<<std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Listas S/N" << std::endl;
	return doQuestion();

}
char ChargeScreen::chargeAdministrator() {
	uint32_t usuario;
	std::string password;

	system("clear");
	std::cout << "Alta de un registro del Archivo de Administrador" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Ingrese el codigo de ususario y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	usuario = IstreamUtils::getUint();
	std::cout << "" << std::endl;
	std::cout << "Ingrese la contraseña y presione ENTER" << std::endl;
	std::cout << "" << std::endl;
	password = IstreamUtils::getString();

	AdministratorFile *aFile = AdministratorFile::getInstance();
	AdministratorRecord record = AdministratorRecord(usuario, password);
	try {
		aFile->insert(record);
	} catch (FileInsertException) {
		std::cout << "Error el Registro ya existe" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Quiere agregar otro registro al archivo de Administrador S/N" << std::endl;
		return doQuestion();
	}
	std::cout << "Se inserto correctamente el siguiente Registo Administrador: ";
	std::cout << record.getKey().getUint() << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Quiere agregar otro registro al archivo de Administrador S/N" << std::endl;
	return doQuestion();
}
char ChargeScreen::doQuestion() {
	while (true) {
		char resp = IstreamUtils::getChar();
		switch (resp) {
		case 'S':
			return resp;
		case 'N':
			return resp;
		default:
			std::cout << "Respuesta no valida use S/N" << std::endl;
		}
	}
}
bool ChargeScreen::validateDNI(uint32_t dni) {
		VoterFile* voteFile = VoterFile::getInstance();
		VoterRecord::Key vKey(dni);
		try{
			user  = voteFile->search(vKey);
			if(user.getDni() == dni)
				return true;
			else
				return false;
		}catch(FileSearchException e)
		{
			return false;
		}
}

