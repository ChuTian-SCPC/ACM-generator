#ifndef _SGPCET_ENUM_H_
#define _SGPCET_ENUM_H_

namespace generator {
  namespace _enum {
    enum class Color {
      Red,
      Green,
      Yellow,
      Grey,
      Reset,
      Default = Reset,
      MaxColor
    };
    
    constexpr int __color_index(Color color) {
      return static_cast<int>(color);
    }
    
    enum class _JudgeState {
      _UNKNOWN,
      _AC,
      _WA,
      _ERROR = 3,
      _TLE = 4,
      _TLE_AC = 5,
      _TLE_WA = 6,
      _JUDGE_STATE_MAX
    };

    constexpr int __state_index(_JudgeState state) {
      return static_cast<int>(state);
    }

    inline _JudgeState& operator++(_JudgeState& s) {
      if (s != _JudgeState::_JUDGE_STATE_MAX) 
        s = static_cast<_JudgeState>(static_cast<int>(s) + 1);
      return s;
    }

    inline _JudgeState operator|(_JudgeState lhs, _JudgeState rhs) {
        return static_cast<_JudgeState>(
            static_cast<int>(lhs) | static_cast<int>(rhs)
        );
    }

    inline _JudgeState& operator|=(_JudgeState& lhs, _JudgeState rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    bool __has_ac(_JudgeState state) {
      return state == _JudgeState::_AC || state == _JudgeState::_TLE_AC;
    }

    bool __has_wa(_JudgeState state) {
      return state == _JudgeState::_WA || state == _JudgeState::_TLE_WA;
    }
    
    bool __is_tle(_JudgeState state) {
      return state == _JudgeState::_TLE;
    }
    
    bool __has_tle(_JudgeState state) {
      return __is_tle(state) || state == _JudgeState::_TLE_AC || state == _JudgeState::_TLE_WA;
    }

    bool __is_combine_state(_JudgeState state) {
      return state == _JudgeState::_TLE_AC || state == _JudgeState::_TLE_WA;
    }
    
    bool __is_run_error(_JudgeState state) {
      return state == _JudgeState::_ERROR;
    }

    bool __is_unknown(_JudgeState state) {
      return state == _JudgeState::_UNKNOWN;
    }

    bool __is_consider_state(_JudgeState state) {
      return !__is_unknown(state) && !__is_run_error(state);
    }

    enum _End{
      _IN,
      _OUT,
      _ANS,
      _LOG,
      _LOGC,
      _EXE,
      _VAL,
      _CHECK_RESULT,
      _MAX_END  
    };

    enum _FuncProgramType {
      _GENERATOR,
      _CHECKER,
      _VALIDATOR,
      _RESULT,
      _OTHER  
    };

    enum Checker{
      lcmp,
      yesno,
      rcmp4,
      rcmp6,
      rcmp9,
      wcmp,
      MaxChecker
    };

    enum CharType{
      LowerLetter,
      UpperLetter,
      Letter,
      Number,
      LetterNumber,
      ZeroOne,
      MaxCharType
    };

    enum TreeGenerator {
      RandomFather,
      Pruefer
    };

    enum class LinkType {
      Direct,
      Increase,
      Shuffle,           
      Dedupe
    };
    
    using MergeType = LinkType;
    
    enum class TreeLinkType {
      Direct,
      Increase,
      Shuffle            
    };

    enum PointDirection {
      COUNTER_CLOCKWISE,
      CLOCKWISE,
      ONLINE_BACK,
      ONLINE_FRONT,
      ON_SEGMENT
    };

    enum VectorOrder {
      Ascending,  // 递增
      Descending, // 递减
      Random
    };

    enum VectorUniqueness {
      Distinct, // 不重复
      Duplicate // 允许重复
    };
  } // namespace _enum
} // namespace generator

#endif // !_SGPCET_ENUM_H_
