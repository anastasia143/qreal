#include "qrtext/src/lua/luaInterpreter.h"

#include "qrtext/lua/types/string.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/tableConstructor.h"

#include "qrtext/lua/ast/unaryMinus.h"
#include "qrtext/lua/ast/not.h"
#include "qrtext/lua/ast/length.h"
#include "qrtext/lua/ast/bitwiseNegation.h"

#include "qrtext/lua/ast/addition.h"
#include "qrtext/lua/ast/subtraction.h"
#include "qrtext/lua/ast/multiplication.h"
#include "qrtext/lua/ast/division.h"
#include "qrtext/lua/ast/integerDivision.h"
#include "qrtext/lua/ast/exponentiation.h"
#include "qrtext/lua/ast/modulo.h"
#include "qrtext/lua/ast/bitwiseAnd.h"
#include "qrtext/lua/ast/bitwiseXor.h"
#include "qrtext/lua/ast/bitwiseOr.h"
#include "qrtext/lua/ast/bitwiseRightShift.h"
#include "qrtext/lua/ast/bitwiseLeftShift.h"
#include "qrtext/lua/ast/concatenation.h"
#include "qrtext/lua/ast/lessThan.h"
#include "qrtext/lua/ast/greaterThan.h"
#include "qrtext/lua/ast/lessOrEqual.h"
#include "qrtext/lua/ast/greaterOrEqual.h"
#include "qrtext/lua/ast/equality.h"
#include "qrtext/lua/ast/inequality.h"
#include "qrtext/lua/ast/logicalAnd.h"
#include "qrtext/lua/ast/logicalOr.h"

using namespace qrtext::lua::details;
using namespace qrtext;

QVariant LuaInterpreter::interpret(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	Q_UNUSED(semanticAnalyzer);

	QVariant result;

	/// @todo Integer and float literals may differ from those recognized in toInt() and toDouble().
	if (root->is<ast::IntegerNumber>()) {
		result = as<ast::IntegerNumber>(root)->stringRepresentation().toInt();
	}

	if (root->is<ast::FloatNumber>()) {
		result = as<ast::FloatNumber>(root)->stringRepresentation().toDouble();
	}

	if (root->is<ast::String>()) {
		result = as<ast::String>(root)->string();
	}

	if (root->is<ast::TableConstructor>()) {
		QStringList temp;
		for (auto node : as<ast::TableConstructor>(root)->initializers()) {
			temp << interpret(node->value(), semanticAnalyzer).value<QString>();
		}

		result = QVariant(temp);
	}

	if (root->is<ast::Assignment>()) {
		auto variable = as<ast::Assignment>(root)->variable();
		auto value = as<ast::Assignment>(root)->value();
		auto interpretedValue = interpret(value, semanticAnalyzer);

		if (variable->is<ast::Identifier>()) {
			auto name = as<ast::Identifier>(variable)->name();
			mIdentifierValues.insert(name, interpretedValue);
		}

		/// @todo Assignment is also possible to array slice, for example "a[1] = 1", or more complex case,
		///       "f(x)[1] = 1". Note that field access in form of "a.x = 1" is parsed as "a['x'] = 1", so no special
		///       handling is needed for that case.
	}

	if (root->is<ast::Identifier>()) {
		result = mIdentifierValues.value(as<ast::Identifier>(root)->name());
	}

	if (root->is<ast::FunctionCall>()) {
		auto function = as<ast::FunctionCall>(root)->function();
		auto name = as<ast::Identifier>(function)->name();
		auto parameters = as<ast::FunctionCall>(root)->arguments();

		QList<QVariant> actualParameters;
		for (auto parameter : parameters) {
			actualParameters << interpret(parameter, semanticAnalyzer);
		}

		result = mIntrinsicFunctions[name](actualParameters);
	}

	if (root->is<ast::UnaryOperator>()) {
		interpretUnaryOperator(root, semanticAnalyzer);
	}

	if (root->is<ast::BinaryOperator>()) {
		interpretBinaryOperator(root, semanticAnalyzer);
	}

	return result;
}

void LuaInterpreter::addIntrinsicFunction(QString const &name
		, std::function<QVariant(QList<QVariant> const &)> const &semantic)
{
	mIntrinsicFunctions.insert(name, semantic);
}

QVariant LuaInterpreter::interpretUnaryOperator(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	auto operand = as<ast::UnaryOperator>(root)->operand();
	if (root->is<ast::UnaryMinus>()) {
		return -interpret(operand, semanticAnalyzer).toFloat();
	} else if (root->is<ast::Not>()) {
		QVariant const operandResult = interpret(operand, semanticAnalyzer);
		/// @todo Code 'nil' more adequately.
		if (operandResult.isNull()) {
			return true;
		}

		return !operandResult.toBool();
	} else if (root->is<ast::Length>()) {
		if (semanticAnalyzer.type(operand)->is<types::String>()) {
			/// @todo Well, in Lua '#' returns bytes in a string, not symbols.
			return interpret(operand, semanticAnalyzer).toString().length();
		}
		/// @todo Support everything else.
	} else if (root->is<ast::BitwiseNegation>()) {
		return ~(interpret(operand, semanticAnalyzer).toInt());
	}

	return QVariant();
}

QVariant LuaInterpreter::interpretBinaryOperator(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	return QVariant();
}