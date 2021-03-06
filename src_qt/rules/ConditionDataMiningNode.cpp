#include <QtCore/QString>

#include "ConditionDataMiningNode.h"
#include "../Brain.h"


namespace CnotiMind
{

	ConditionDataMiningNode::ConditionDataMiningNode(const QString& key, const QString& value,
													 ConditionOperator op, DataMiningOperation dataMiningOperation,
													 MemoryType memory, const QString& variable,
													 const QString& compareValue, Brain* brain,
													 QObject* parent):
		ConditionNode( key, value, op, brain, parent ),
		_dataMiningOperation( dataMiningOperation ),
		_variable( variable ),
		_compareValue( compareValue ),
		_memory( memory ),
		_isCompareValueNumeric( false )
	{
		_compareValueNumeric = _compareValue.toFloat( &_isCompareValueNumeric );
	}

	void ConditionDataMiningNode::exec()
	{
		if( isTrue() )
		{
			if( _variable.isEmpty() )
			{
				execChildren();
			}
			else
			{
				QHash<QString,QString> variables;
				variables.insert( _variable, _result );
				execChildren( variables );
			}
		}
	}

	void ConditionDataMiningNode::exec( QHash<QString, QString> &variables )
	{
		if( isTrue() )
		{
			if( !_variable.isEmpty() )
			{
				variables.insert( _variable, _result );
			}
			execChildren( variables );
		}
	}

	QString ConditionDataMiningNode::info( int depth ) const
	{
		QString info;

		info += space(depth) + "Condition type=DataMining";

		return info + RuleNode::info(depth);
	}

	/*
		Test if the datamining condition is true
	*/
	bool ConditionDataMiningNode::isTrue()
	{
		bool valid;

		// If the values are numbers it should use the
		if( _isValueNumeric && _isCompareValueNumeric )
		{
			QVariant result = _brain->dataMining( _dataMiningOperation, _key, _valueNumeric, 0, _memory, &valid );

			if(!valid)
			{
				return false;
			}

			_result = result.toString();
			qreal _resultNumeric = result.toReal();

			switch( _operator )
			{
				case ConditionOperatorBigger: return _resultNumeric > _compareValueNumeric ;
				case ConditionOperatorBiggerOrEqual: return _resultNumeric >= _compareValueNumeric;
				case ConditionOperatorSmaller: return _resultNumeric < _compareValueNumeric;
				case ConditionOperatorSmallerOrEqual: return _resultNumeric <= _compareValueNumeric;
				case ConditionOperatorEqual: return _resultNumeric == _compareValueNumeric;
				case ConditionOperatorDifferent: return _resultNumeric != _compareValueNumeric;
			}

			return false;
		}
		else // If the value is QString
		{
			QVariant result = _brain->dataMining( _dataMiningOperation, _key, _value, 0, _memory, &valid );

			if(!valid)
			{
				return false;
			}

			_result = result.toString();

			switch( _operator )
			{
				case ConditionOperatorEqual: return QString::compare( _result, _compareValue, Qt::CaseInsensitive ) == 0;
				case ConditionOperatorDifferent: return QString::compare( _result, _compareValue, Qt::CaseInsensitive ) != 0;
			}

			return false;
		}
	}

}
