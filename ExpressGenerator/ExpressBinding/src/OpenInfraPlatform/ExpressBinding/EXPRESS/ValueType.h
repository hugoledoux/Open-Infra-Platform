/*
    This file is part of Expresso, a simple early binding generator for EXPRESS.
	Copyright (c) 2016 Technical University of Munich
	Chair of Computational Modeling and Simulation.

    BlueFramework is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    BlueFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#ifndef OpenInfraPlatform_ExpressBinding_ValueType_102447dd_302a_4c8f_bf0d_7515bbdcfc1c_h
#define OpenInfraPlatform_ExpressBinding_ValueType_102447dd_302a_4c8f_bf0d_7515bbdcfc1c_h

#include "OpenInfraPlatform/ExpressBinding/namespace.h"

#include "EXPRESSOptional.h"
#include "EXPRESSContainer.h"
#include "EXPRESSType.h"

#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>


//#include <boost/preprocessor.hpp>
#include <boost/logic/tribool.hpp>

OIP_NAMESPACE_OPENINFRAPLATFORM_EXPRESSBINDING_BEGIN

template <typename T> class ValueType : public EXPRESSType {
//	static_assert(std::is_same<T,boost::logic::tribool>::value || std::is_integral<T>::value || std::is_same<T, std::string>::value || std::is_floating_point<T>::value || std::is_enum<T>::value, "EXPRESSType is undefined for template parameter type.");

public:
	typedef T UnderlyingType;

	ValueType() = default;
	ValueType(const ValueType& other) : m_value(other.m_value) {};
	ValueType(const T& value) : m_value(value) {};

	virtual ~ValueType() { };

	virtual const std::string getStepParameter() const override { return "unknown"; };

	static T readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model = nullptr) { return T::readStepData(value); };

	virtual const std::string classname() const override { return typeid(T).name(); };

	virtual ValueType& operator=(const T& other) { m_value = other; return *this; }
	virtual ValueType& operator=(const ValueType& other) { m_value = other.m_value; return *this; };
	//virtual ValueType& operator=(const Optional<ValueType>& optional) { m_value = optional.get_value_or(T()); return *this; }
	
	virtual operator T&() { return std::ref(m_value); }
	virtual operator const T() const { return m_value; }

	virtual ValueType* operator->() { return this; }
	virtual const ValueType* const operator->() const { return this; }

	//TODO: Operator==

	friend std::ostream& operator<<(std::ostream &out, const ValueType& obj) { out << obj.m_value; return out; }
	friend std::istream& operator>>(std::istream &in, ValueType& obj) { in >> obj.m_value; return in; }
	
protected:
	T m_value;
};

template <typename T> const bool operator==(const ValueType<T>& lhs, const ValueType<T>& rhs) { return (T)lhs == (T)rhs; }
template <typename T> const bool operator==(const ValueType<T>& lhs, const T rhs) { return (T)lhs == (T)rhs; }
template <typename T> const bool operator==(const T lhs, const ValueType<T>& rhs) { return lhs == (T)rhs; }
template <typename T, typename V> const bool operator==(const ValueType<T>& lhs, const V rhs) { return(T)lhs == rhs; }


const std::string ValueType<double>::getStepParameter() const { return std::to_string(ValueType<double>::m_value); };
const std::string ValueType<int>::getStepParameter() const { return std::to_string(ValueType<int>::m_value); };
const std::string ValueType<bool>::getStepParameter() const { return std::to_string(ValueType<bool>::m_value); };
const std::string ValueType<std::string>::getStepParameter() const { return "'" + ValueType<std::string>::m_value + "'"; };
const std::string ValueType<boost::logic::tribool>::getStepParameter() const {
	switch (ValueType<boost::logic::tribool>::m_value.value) {
	case boost::logic::tribool::true_value: return "TRUE";
	case boost::logic::tribool::false_value: return "FALSE";
	case boost::logic::tribool::indeterminate_value: return "UNKNOWN";
	}
	return "ERROR";
}

//template <typename ContainerType, typename ValueType, int MinCardinality, int MaxCardinality> std::string to_string(const EXPRESSContainer<ContainerType, ValueType, MinCardinality, MaxCardinality>& val) {
//	return val.getStepParameter();
//};

//template <typename ValueType, int MinCardinality, int MaxCardinality> std::string to_string(const EXPRESSContainer<ValueType, MinCardinality, MaxCardinality>& val) {
//	return val.getStepParameter();
//};


double ValueType<double>::readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model) { return(stod(value)); };

int ValueType<int>::readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model) { return (stoi(value)); };

bool ValueType<bool>::readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model) {
	std::string lower;
	std::transform(value.begin(), value.end(), lower.begin(), ::tolower);
	return (lower == "true");
};

std::string ValueType<std::string>::readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model) { return (value); };

boost::logic::tribool ValueType<boost::logic::tribool>::readStepData(const std::string &value, const std::shared_ptr<EXPRESSModel>& model) {
	std::string lower;
	std::transform(value.begin(), value.end(), lower.begin(), ::tolower);
	if (lower == "true") {
		return boost::logic::tribool::true_value;
	}
	else if (lower == "false") {
		return boost::logic::tribool::false_value;
	}
	else {
		return boost::logic::tribool(boost::logic::indeterminate);
	}
};


using REAL = ValueType<double>;
using INTEGER = ValueType<int>;
using STRING = ValueType<std::string>;
using BOOLEAN = ValueType<bool>;
using LOGICAL = ValueType<boost::logic::tribool>;
using BINARY = ValueType<int>;
using NUMBER = ValueType<double>;

OIP_NAMESPACE_OPENINFRAPLATFORM_EXPRESSBINDING_END

//#define DEFINE_TYPE(name, basetype)\
//	class name : public basetype {\
//	using base = basetype;\
//	public:\
//		typedef name type;\
//		using base::base;\
//		using base::operator=;\
//		virtual name* operator->() override { return this; }\
//		virtual const name* const operator->() const override { return this; }\
//		virtual name& operator=(const Optional<name> &other) { this->m_value = other.get_value_or(name()); return *this; };\
//		virtual const std::string classname() const override { return #name; }\
//	};

#endif // end define OpenInfraPlatform_ExpressBinding_EXPRESSType_102447dd_302a_4c8f_bf0d_7515bbdcfc1c_h