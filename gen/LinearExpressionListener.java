// Generated from /home/vincenzo/project/rtl-mc/src/parser/antlr4/g4/LinearExpression.g4 by ANTLR 4.13.2
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link LinearExpressionParser}.
 */
public interface LinearExpressionListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link LinearExpressionParser#linearExpr}.
	 * @param ctx the parse tree
	 */
	void enterLinearExpr(LinearExpressionParser.LinearExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link LinearExpressionParser#linearExpr}.
	 * @param ctx the parse tree
	 */
	void exitLinearExpr(LinearExpressionParser.LinearExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link LinearExpressionParser#signTerm}.
	 * @param ctx the parse tree
	 */
	void enterSignTerm(LinearExpressionParser.SignTermContext ctx);
	/**
	 * Exit a parse tree produced by {@link LinearExpressionParser#signTerm}.
	 * @param ctx the parse tree
	 */
	void exitSignTerm(LinearExpressionParser.SignTermContext ctx);
	/**
	 * Enter a parse tree produced by the {@code intTimesVar}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void enterIntTimesVar(LinearExpressionParser.IntTimesVarContext ctx);
	/**
	 * Exit a parse tree produced by the {@code intTimesVar}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void exitIntTimesVar(LinearExpressionParser.IntTimesVarContext ctx);
	/**
	 * Enter a parse tree produced by the {@code var}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void enterVar(LinearExpressionParser.VarContext ctx);
	/**
	 * Exit a parse tree produced by the {@code var}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void exitVar(LinearExpressionParser.VarContext ctx);
	/**
	 * Enter a parse tree produced by the {@code int}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void enterInt(LinearExpressionParser.IntContext ctx);
	/**
	 * Exit a parse tree produced by the {@code int}
	 * labeled alternative in {@link LinearExpressionParser#term}.
	 * @param ctx the parse tree
	 */
	void exitInt(LinearExpressionParser.IntContext ctx);
}