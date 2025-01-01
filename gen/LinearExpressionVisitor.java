// Generated from /home/vincenzo/project/rtl-mc/src/parser/antlr4/g4/LinearExpression.g4 by ANTLR 4.13.2
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link LinearExpressionParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface LinearExpressionVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link LinearExpressionParser#linearExpr}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLinearExpr(LinearExpressionParser.LinearExprContext ctx);
	/**
	 * Visit a parse tree produced by {@link LinearExpressionParser#signTerm}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSignTerm(LinearExpressionParser.SignTermContext ctx);
	/**
	 * Visit a parse tree produced by the {@code intTimesVar}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitIntTimesVar(LinearExpressionParser.IntTimesVarContext ctx);
	/**
	 * Visit a parse tree produced by the {@code var}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitVar(LinearExpressionParser.VarContext ctx);
	/**
	 * Visit a parse tree produced by the {@code int}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitInt(LinearExpressionParser.IntContext ctx);
}