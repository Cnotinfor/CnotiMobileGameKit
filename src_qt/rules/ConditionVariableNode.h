#ifndef CONDITIONVARIABLENODE_H
#define CONDITIONVARIABLENODE_H

#include "ConditionNode.h"

namespace CnotiMind
{

	class ConditionVariableNode : public ConditionNode
	{
		Q_OBJECT
	public:

		ConditionVariableNode(const QString& variable, const QString& value, ConditionOperator op, Brain* brain, QObject* parent);

		void exec();
		void exec( QHash<QString,QString>& variables );

		QString info( int depth = 0 ) const;

	protected:
		bool isTrue();
		bool isTrue( QHash<QString,QString>& variables );

	};

}
#endif // CONDITIONVARIABLENODE_H
