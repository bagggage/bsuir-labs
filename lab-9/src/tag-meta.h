// Copyright: 2023 Pigulevskiy Konstantin
// Lisence: MIT

#pragma once

#include "bool.h"

#define NULL 0

typedef struct TagMeta
{
	const char* name;
	const char* class;
	const char* specificIdentifier;
	const char* specificIdentifierValue;
	const char** dataIdentifiers;

	unsigned int dataIdentifiersCount;

	/*If true class name should be equals to 'tagClass', to pass validation.
	Otherwise parser compare only first part of the class name.*/
	BOOL isClassStongCompare;

	/*If true all identifier should be equals to 'specificIdentifierValue', to pass validation.
	Otherwise parser compare only first part of identifier.*/
	BOOL isIdentifierStrongCompare;

	/*If true, after finding a suitable tag, takes data only from the first deepest child tag.*/
	BOOL isShouldGetFirstDeeperData;

	/*If true get data from identifier that equals 'dataIdentifier'.
	'isShouldGetFirstDeeperData' is ignored.*/
	BOOL isShouldGetDataFromIdentifiers;

} TagMeta;

TagMeta makeTag(const char* name, const char* class);