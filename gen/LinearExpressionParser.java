// Generated from /home/vincenzo/project/rtl-mc/src/parser/antlr4/g4/LinearExpression.g4 by ANTLR 4.13.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class LinearExpressionParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		LINE_COMMENT=1, COMMENT=2, WS=3, UNSIGNED_INT=4, NEWLINE=5, TIMES=6, PLUS=7, 
		MINUS=8, LE=9, LT=10, GT=11, GE=12, EQ=13, FLOW=14, INV=15, ID=16, ANY_ID=17;
	public static final int
		RULE_linearExpr = 0, RULE_signTerm = 1, RULE_term = 2;
	private static String[] makeRuleNames() {
		return new String[] {
			"linearExpr", "signTerm", "term"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, null, null, null, null, null, "'*'", "'+'", "'-'", "'<='", "'<'", 
			"'>'", "'>='", "'='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "LINE_COMMENT", "COMMENT", "WS", "UNSIGNED_INT", "NEWLINE", "TIMES", 
			"PLUS", "MINUS", "LE", "LT", "GT", "GE", "EQ", "FLOW", "INV", "ID", "ANY_ID"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "LinearExpression.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public LinearExpressionParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LinearExprContext extends ParserRuleContext {
		public Token op;
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public List<SignTermContext> signTerm() {
			return getRuleContexts(SignTermContext.class);
		}
		public SignTermContext signTerm(int i) {
			return getRuleContext(SignTermContext.class,i);
		}
		public TerminalNode PLUS() { return getToken(LinearExpressionParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(LinearExpressionParser.MINUS, 0); }
		public LinearExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_linearExpr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).enterLinearExpr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).exitLinearExpr(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof LinearExpressionVisitor ) return ((LinearExpressionVisitor<? extends T>)visitor).visitLinearExpr(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LinearExprContext linearExpr() throws RecognitionException {
		LinearExprContext _localctx = new LinearExprContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_linearExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(7);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==PLUS || _la==MINUS) {
				{
				setState(6);
				((LinearExprContext)_localctx).op = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
					((LinearExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			setState(9);
			term();
			setState(13);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PLUS || _la==MINUS) {
				{
				{
				setState(10);
				signTerm();
				}
				}
				setState(15);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SignTermContext extends ParserRuleContext {
		public Token op;
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public TerminalNode PLUS() { return getToken(LinearExpressionParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(LinearExpressionParser.MINUS, 0); }
		public SignTermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_signTerm; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).enterSignTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).exitSignTerm(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof LinearExpressionVisitor ) return ((LinearExpressionVisitor<? extends T>)visitor).visitSignTerm(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SignTermContext signTerm() throws RecognitionException {
		SignTermContext _localctx = new SignTermContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_signTerm);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(16);
			((SignTermContext)_localctx).op = _input.LT(1);
			_la = _input.LA(1);
			if ( !(_la==PLUS || _la==MINUS) ) {
				((SignTermContext)_localctx).op = (Token)_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(17);
			term();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TermContext extends ParserRuleContext {
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
	 
		public TermContext() { }
		public void copyFrom(TermContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class IntTimesVarContext extends TermContext {
		public TerminalNode UNSIGNED_INT() { return getToken(LinearExpressionParser.UNSIGNED_INT, 0); }
		public TerminalNode ID() { return getToken(LinearExpressionParser.ID, 0); }
		public TerminalNode TIMES() { return getToken(LinearExpressionParser.TIMES, 0); }
		public IntTimesVarContext(TermContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).enterIntTimesVar(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).exitIntTimesVar(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof LinearExpressionVisitor ) return ((LinearExpressionVisitor<? extends T>)visitor).visitIntTimesVar(this);
			else return visitor.visitChildren(this);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class VarContext extends TermContext {
		public TerminalNode ID() { return getToken(LinearExpressionParser.ID, 0); }
		public VarContext(TermContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).enterVar(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).exitVar(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof LinearExpressionVisitor ) return ((LinearExpressionVisitor<? extends T>)visitor).visitVar(this);
			else return visitor.visitChildren(this);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class IntContext extends TermContext {
		public TerminalNode UNSIGNED_INT() { return getToken(LinearExpressionParser.UNSIGNED_INT, 0); }
		public IntContext(TermContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).enterInt(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof LinearExpressionListener ) ((LinearExpressionListener)listener).exitInt(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof LinearExpressionVisitor ) return ((LinearExpressionVisitor<? extends T>)visitor).visitInt(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_term);
		int _la;
		try {
			setState(26);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,3,_ctx) ) {
			case 1:
				_localctx = new IntTimesVarContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(19);
				match(UNSIGNED_INT);
				setState(21);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TIMES) {
					{
					setState(20);
					match(TIMES);
					}
				}

				setState(23);
				match(ID);
				}
				break;
			case 2:
				_localctx = new VarContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(24);
				match(ID);
				}
				break;
			case 3:
				_localctx = new IntContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(25);
				match(UNSIGNED_INT);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u0011\u001d\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001"+
		"\u0002\u0002\u0007\u0002\u0001\u0000\u0003\u0000\b\b\u0000\u0001\u0000"+
		"\u0001\u0000\u0005\u0000\f\b\u0000\n\u0000\f\u0000\u000f\t\u0000\u0001"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002\u0003\u0002\u0016"+
		"\b\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0003\u0002\u001b\b\u0002"+
		"\u0001\u0002\u0000\u0000\u0003\u0000\u0002\u0004\u0000\u0001\u0001\u0000"+
		"\u0007\b\u001e\u0000\u0007\u0001\u0000\u0000\u0000\u0002\u0010\u0001\u0000"+
		"\u0000\u0000\u0004\u001a\u0001\u0000\u0000\u0000\u0006\b\u0007\u0000\u0000"+
		"\u0000\u0007\u0006\u0001\u0000\u0000\u0000\u0007\b\u0001\u0000\u0000\u0000"+
		"\b\t\u0001\u0000\u0000\u0000\t\r\u0003\u0004\u0002\u0000\n\f\u0003\u0002"+
		"\u0001\u0000\u000b\n\u0001\u0000\u0000\u0000\f\u000f\u0001\u0000\u0000"+
		"\u0000\r\u000b\u0001\u0000\u0000\u0000\r\u000e\u0001\u0000\u0000\u0000"+
		"\u000e\u0001\u0001\u0000\u0000\u0000\u000f\r\u0001\u0000\u0000\u0000\u0010"+
		"\u0011\u0007\u0000\u0000\u0000\u0011\u0012\u0003\u0004\u0002\u0000\u0012"+
		"\u0003\u0001\u0000\u0000\u0000\u0013\u0015\u0005\u0004\u0000\u0000\u0014"+
		"\u0016\u0005\u0006\u0000\u0000\u0015\u0014\u0001\u0000\u0000\u0000\u0015"+
		"\u0016\u0001\u0000\u0000\u0000\u0016\u0017\u0001\u0000\u0000\u0000\u0017"+
		"\u001b\u0005\u0010\u0000\u0000\u0018\u001b\u0005\u0010\u0000\u0000\u0019"+
		"\u001b\u0005\u0004\u0000\u0000\u001a\u0013\u0001\u0000\u0000\u0000\u001a"+
		"\u0018\u0001\u0000\u0000\u0000\u001a\u0019\u0001\u0000\u0000\u0000\u001b"+
		"\u0005\u0001\u0000\u0000\u0000\u0004\u0007\r\u0015\u001a";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}