#ifndef Match_TOKEN_H
#define Match_TOKEN_H


inline EmType MatchTokenInt();
inline void MatchTokenMain();
inline void MatchTokenBreak();
inline void MatchTokenContinue();
inline void MatchTokenIf();
inline void MatchTokenElse();
inline void MatchTokenWhile();
inline void MatchTokenIdent(const bool& isDecl = false);
inline void MatchTokenGetint();
inline void MatchTokenPrintf();
inline void MatchTokenReturn();
inline void MatchTokenVoid();
inline void MatchTokenFormat();
inline void MatchTokenConst();
inline void MatchTokenComma();
inline void MatchTokenSemicn(const emErrorType& errorType = Null);
inline void MatchTokenAssign();
inline void MatchTokenLss();
inline void MatchTokenLeq();
inline void MatchTokenGre();
inline void MatchTokenGeq();
inline void MatchTokenEql();
inline void MatchTokenNeq();
inline void MatchTokenNot();
inline void MatchTokenAnd();
inline void MatchTokenOr();
inline void MatchTokenPlus();
inline void MatchTokenMinu();
inline void MatchTokenMult();
inline void MatchTokenDiv();
inline void MatchTokenMod();
inline void MatchTokenLparent();
inline void MatchTokenRparent(const emErrorType& errorType = Null);
inline void MatchTokenLbrace();
inline void MatchTokenRbrace();
inline void MatchTokenLbrack();
inline void MatchTokenRbrack(const emErrorType& errorType = Null);
inline void MatchTokenZero();
inline void MatchTokenNonZeroDigit();
inline void MatchTokenDigit();

#endif