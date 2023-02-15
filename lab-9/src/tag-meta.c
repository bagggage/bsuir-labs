// Copyright: 2023 Pigulevskiy Konstantin
// Lisence: MIT

#include "tag-meta.h"

TagMeta makeTag(const char* name, const char* class) 
{
	TagMeta tag = 
	{ 
		.name = name,
		.class = class,
		.specificIdentifier = NULL,
		.specificIdentifierValue = NULL,
		.dataIdentifiers = NULL,
		.dataIdentifiersCount = 0,
		.isClassStongCompare = TRUE,
		.isIdentifierStrongCompare = TRUE,
		.isShouldGetFirstDeeperData = FALSE,
		.isShouldGetDataFromIdentifiers = FALSE
	};

	return tag;
}
