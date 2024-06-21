/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		QualifiedName.h
 *	DESCRIPTION:	Qualified metadata name holder.
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_QUALIFIEDNAME_H
#define JRD_QUALIFIEDNAME_H

#include "../jrd/MetaName.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_pair.h"
#include "../common/StatusArg.h"

namespace Jrd {

class QualifiedName
{
public:
	explicit QualifiedName(MemoryPool& p, const MetaName& aObject,
			const MetaName& aSchema = {}, const MetaName& aPackage = {})
		: object(p, aObject),
		  schema(p, aSchema),
		  package(p, aPackage)
	{
	}

	explicit QualifiedName(const MetaName& aObject, const MetaName& aSchema = {}, const MetaName& aPackage = {})
		: object(aObject),
		  schema(aSchema),
		  package(aPackage)
	{
	}

	QualifiedName(MemoryPool& p, const QualifiedName& src)
		: object(p, src.object),
		  schema(p, src.schema),
		  package(p, src.package)
	{
	}

	QualifiedName(const QualifiedName& src)
		: object(src.object),
		  schema(src.schema),
		  package(src.package)
	{
	}

	explicit QualifiedName(MemoryPool& p)
		: object(p),
		  schema(p),
		  package(p)
	{
	}

	QualifiedName()
	{
	}

public:
	bool operator<(const QualifiedName& m) const
	{
		return schema < m.schema ||
			(schema == m.schema && object < m.object) ||
			(schema == m.schema && object == m.object && package < m.package);
	}

	bool operator>(const QualifiedName& m) const
	{
		return schema > m.schema ||
			(schema == m.schema && object > m.object) ||
			(schema == m.schema && object == m.object && package > m.package);
	}

	bool operator==(const QualifiedName& m) const
	{
		return schema == m.schema && object == m.object && package == m.package;
	}

	bool operator!=(const QualifiedName& m) const
	{
		return !(*this == m);
	}

public:
	QualifiedName getSchemaAndPackage() const
	{
		return QualifiedName(package, schema);
	}

	/* FIXME:
	QualifiedName withPackage(const MetaName& newPackage) const
	{
		return QualifiedName(object, schema, newPackage);
	}
	*/

	void clear()
	{
		object = {};
		schema = {};
		package = {};
	}

	Firebird::string toString() const
	{
		// FIXME: Quote names when necessary?

		Firebird::string s;

		if (schema.hasData())
		{
			s = schema.c_str();
			s.append(".");
		}

		if (package.hasData())
		{
			s.append(package.c_str());
			s.append(".");
		}

		s.append(object.c_str());

		return s;
	}

public:
	MetaName object;
	MetaName schema;
	MetaName package;
};

using QualifiedNameMetaNamePair = Firebird::FullPooledPair<QualifiedName, MetaName>;

} // namespace Jrd

#endif // JRD_QUALIFIEDNAME_H
