//
// Row.h
//
// $Id: //poco/Main/Data/include/Poco/Data/Row.h#1 $
//
// Library: Data
// Package: DataCore
// Module:  Row
//
// Definition of the Row class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_Row_INCLUDED
#define Data_Row_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/DynamicAny.h"
#include "Poco/Tuple.h"
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>
#include <sstream>


namespace Poco {
namespace Data {


class RecordSet;


class Data_API Row
	/// Row class provides a data type for RecordSet iteration purposes.
	/// Dereferencing a RowIterator returns Row.
	/// Rows are sortable. The sortability is maintained at all times (i.e. there
	/// is always at least one column specified as a sorting criteria) .
	/// The default and minimal sorting criteria is the first field (position 0).
	/// The default sorting criteria can be replaced with any other field by 
	/// calling replaceSortField() member function.
	/// Additional fields can be added to sorting criteria, in which case the
	/// field precedence corresponds to addition order (i.e. later added fields
	/// have lower sorting precedence).
	/// These features make Row suitable for use with standard sorted 
	/// containers and algorithms. The main constraint is that all the rows from
	/// a set that is being sorted must have the same sorting criteria (i.e., the same
	/// set of fields must be in sorting criteria in the same order). Since rows don't
	/// know about each other, it is the programmer's responsibility to ensure this
	/// constraint is satisfied.
	/// Field names are a shared pointer to a vector of strings. For efficiency sake,
	/// a constructor taking a shared pointer to names vector argument is provided.
	/// The stream operator is provided for Row data type as a free-standing function.
{
public:
	typedef std::vector<std::string> NameVec;
	typedef SharedPtr<std::vector<std::string> > NameVecPtr;

	enum ComparisonType
	{
		COMPARE_AS_INTEGER,
		COMPARE_AS_FLOAT,
		COMPARE_AS_STRING
	};

	static const std::string EOL;

	Row();
		/// Creates the Row.

	explicit Row(NameVecPtr pNames);
		/// Creates the Row.

	~Row();
		/// Destroys the Row.

	DynamicAny& get(std::size_t col);
		/// Returns the reference to data value at column location.

	DynamicAny& operator [] (std::size_t col);
		/// Returns the reference to data value at column location.

	DynamicAny& operator [] (const std::string& name);
		/// Returns the reference to data value at named column location.

	template <typename T>
	void append(const std::string& name, const T& val)
		/// Appends the value to the row.
	{
		if (!_pNames) _pNames = new NameVec;
		DynamicAny da = val;
		_values.push_back(da);
		_pNames->push_back(name);
		if (1 == _values.size()) addSortField(0);
	}
	
	template <typename T>
	void set(std::size_t pos, const T& val)
		/// Assigns the value to the row.
	{
		try
		{
			_values.at(pos) = val;
		}catch (std::out_of_range&)
		{
			throw RangeException("Invalid column number.");
		}
	}

	template <typename T>
	void set(const std::string& name, const T& val)
		/// Assigns the value to the row.
	{
		NameVec::iterator it = _pNames->begin();
		NameVec::iterator end = _pNames->end();
		for (int i = 0; it != end; ++it, ++i)
		{
			if (*it == name)
				return set(i, val);
		}

		std::ostringstream os;
		os << "Column with name " << name << " not found.";
		throw NotFoundException(os.str());
	}

	std::size_t fieldCount() const;
		/// Returns the number of fields in this row.

	void reset();
		/// Resets the row by clearing all field names and values.

	void separator(const std::string& sep);
		/// Sets the separator.

	void addSortField(std::size_t pos);
		/// Adds the field used for sorting.

	void addSortField(const std::string& name);
		/// Adds the field used for sorting.

	void removeSortField(std::size_t pos);
		/// Removes the field used for sorting.

	void removeSortField(const std::string& name);
		/// Removes the field used for sorting.

	void replaceSortField(std::size_t oldPos, std::size_t newPos);
		/// Replaces the field used for sorting.

	void replaceSortField(const std::string& oldName, const std::string& newName);
		/// Replaces the field used for sorting.

	void resetSort();
		/// Resets the sorting criteria to field 0 only.

	const std::string namesToString() const;
		/// Converts the row names to string, inserting separator
		/// string between fields and end-of-line at the end.

	const std::string valuesToString() const;
		/// Converts the row values to string, inserting separator
		/// string between fields and end-of-line at the end.

	bool operator == (const Row& other) const;
		/// Equality operator.

	bool operator != (const Row& other) const;
		/// Inequality operator.

	bool operator < (const Row& other) const;
		/// Less-then operator.

	NameVecPtr names();
		/// Returns the shared pointer to names vector.

private:
	typedef std::vector<DynamicAny> ValueVec;
	typedef Tuple<std::size_t, ComparisonType> SortTuple;
	typedef std::vector<SortTuple> SortMap;
		/// The type for map holding fields used for sorting criteria.
		/// Fields are added sequentially and have precedence that
		/// corresponds to adding order rather than field's position in the row.
		/// That requirement rules out use of std::map due to its sorted nature.

	std::size_t getPosition(const std::string& name);
	bool isEqualSize(const Row& other) const;
	bool isEqualType(const Row& other) const;

	std::string         _separator;
	NameVecPtr          _pNames;
	ValueVec            _values;
	SortMap             _sortFields;
};


Data_API std::ostream& operator << (std::ostream &os, const Row& row);


///
/// inlines
///
inline std::size_t Row::fieldCount() const
{
	return _values.size();
}


inline void Row::reset()
{
	_pNames->clear();
	_values.clear();
}


inline void Row::separator(const std::string& sep)
{
	_separator = sep;
}


inline Row::NameVecPtr Row::names()
{
	return _pNames;
}


inline DynamicAny& Row::operator [] (std::size_t col)
{
	return get(col);
}


inline DynamicAny& Row::operator [] (const std::string& name)
{
	return get(getPosition(name));
}


} } // namespace Poco::Data


#endif // Data_Row_INCLUDED
