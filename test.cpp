#include "test.h"
#include "regex_parser.h"

TEST_F(BadRegexTest, Empty) {
    EXPECT_ANY_THROW(MaxSubwordInLanguage("", ""));
}

TEST_F(BadRegexTest, FewDotOperands) {
    EXPECT_ANY_THROW(MaxSubwordInLanguage(".", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("a.", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("abc...", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("bc..", "abcabc"));
}

TEST_F(BadRegexTest, FewConcatOperands) {
    EXPECT_ANY_THROW(MaxSubwordInLanguage("+", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("a+", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("abc+++", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("bc++", "abcabc"));
}

TEST_F(BadRegexTest, FewStarOperands) {
    EXPECT_ANY_THROW(MaxSubwordInLanguage("*", "abcabc"));
}

TEST_F(BadRegexTest, FewOperators) {
    EXPECT_ANY_THROW(MaxSubwordInLanguage("ab", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("abc", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("abc+", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("abc+ab.", "abcabc"));
    EXPECT_ANY_THROW(MaxSubwordInLanguage("bc***", "abcabc"));
}

TEST_F(WordTest, EmptyWord) {
    EXPECT_EQ(MaxSubwordInLanguage("abc.+", ""), 0);
}

TEST_F(SubwordTest, LenTest) {
    {
        const Subword sw = {0, 5};
        EXPECT_EQ(sw.Len(), 5);
    }

    {
        const Subword sw = {0, 0};
        EXPECT_EQ(sw.Len(), 0);
    }

    {
        const Subword sw = {10, 17};
        EXPECT_EQ(sw.Len(), 7);
    }
}

TEST_F(SubwordTest, EmptyTest) {
    {
        const Subword sw = {0, 5};
        EXPECT_FALSE(sw.Empty());
    }

    {
        const Subword sw = {0, 0};
        EXPECT_TRUE(sw.Empty());
    }

    {
        const Subword sw = {10, 17};
        EXPECT_FALSE(sw.Empty());
    }

    {
        const Subword sw = {6, 6};
        EXPECT_TRUE(sw.Empty());
    }
}

TEST_F(SubwordTest, EqualTest) {
    {
        const Subword sw = {0, 5};
        const Subword sw2 = {0, 5};
        EXPECT_TRUE(sw == sw2);
        EXPECT_TRUE(sw2 == sw);
    }

    {
        const Subword sw = {0, 5};
        const Subword sw2 = {0, 7};
        EXPECT_FALSE(sw == sw2);
        EXPECT_FALSE(sw2 == sw);
    }

    {
        const Subword sw = {0, 5};
        const Subword sw2 = {3, 5};
        EXPECT_FALSE(sw == sw2);
        EXPECT_FALSE(sw2 == sw);
    }

    {
        const Subword sw = {5, 5};
        const Subword sw2 = {8, 8};
        EXPECT_TRUE(sw == sw2);
        EXPECT_TRUE(sw2 == sw);
    }
}

TEST_F(SubwordTest, ComparisonTest) {
    {
        const Subword sw = {0, 5};
        const Subword sw2 = {0, 5};
        EXPECT_FALSE(sw < sw2);
        EXPECT_FALSE(sw2 < sw);
    }

    {
        const Subword sw = {0, 5};
        const Subword sw2 = {1, 7};
        EXPECT_TRUE(sw < sw2);
        EXPECT_FALSE(sw2 < sw);
    }

    {
        const Subword sw = {3, 4};
        const Subword sw2 = {3, 5};
        EXPECT_TRUE(sw < sw2);
        EXPECT_FALSE(sw2 < sw);
    }

    {
        const Subword sw = {5, 5};
        const Subword sw2 = {8, 8};
        EXPECT_FALSE(sw < sw2);
        EXPECT_FALSE(sw2 < sw);
    }
}

TEST_F(SubwordTest, ConcatsTest) {
    {
        const Subword sw = {0, 5};
        const Subword sw2 = {0, 5};
        EXPECT_FALSE(sw.Concats(sw2));
        EXPECT_FALSE(sw2.Concats(sw));
    }

    {
        const Subword sw = {0, 5};
        const Subword sw2 = {5, 7};
        EXPECT_TRUE(sw.Concats(sw2));
        EXPECT_FALSE(sw2.Concats(sw));
    }

    {
        const Subword sw = {4, 6};
        const Subword sw2 = {2, 4};
        EXPECT_FALSE(sw.Concats(sw2));
        EXPECT_TRUE(sw2.Concats(sw));
    }

    {
        const Subword sw = {6, 6};
        const Subword sw2 = {2, 4};
        EXPECT_TRUE(sw.Concats(sw2));
        EXPECT_TRUE(sw2.Concats(sw));
    }
}

TEST_F(SubwordTest, ConcatTest) {
    {
        Subword sw = {0, 5};
        const Subword sw2 = {0, 5};
        EXPECT_ANY_THROW(sw += sw2);
    }

    {
        Subword sw = {0, 5};
        const Subword sw2 = {5, 7};
        const Subword r = {0, 7};
        sw += sw2;
        EXPECT_EQ(sw, r);
    }

    {
        Subword sw = {10, 20};
        const Subword sw2 = {20, 31};
        const Subword r = {10, 31};
        sw += sw2;
        EXPECT_EQ(sw, r);
    }

    {
        Subword sw = {10, 20};
        const Subword sw2 = {3, 3};
        const Subword r = {10, 20};
        sw += sw2;
        EXPECT_EQ(sw, r);
    }
}

TEST_F(AnswerTest, SimpleTest) {
  {
    std::string regex = "a";
    std::string word = "a";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 1);
  }

  {
    std::string regex = "b";
    std::string word = "a";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 0);
  }

  {
    std::string regex = "aa.";
    std::string word = "a";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 1);
  }

  {
    std::string regex = "aa.";
    std::string word = "aa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 2);
  }

  {
    std::string regex = "ab.";
    std::string word = "aa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 1);
  }

  {
    std::string regex = "aa.a+";
    std::string word = "aa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 2);
  }

  {
    std::string regex = "aa.a+";
    std::string word = "aaaaaa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 2);
  }

  {
    std::string regex = "ca.a.ca.+ba.a.b.."; // (caa + ca) baab
    std::string word = "abaab";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 5);
  }

  {
    std::string regex = "ca.ca.a.+ba.a.b.."; // (ca + caa) baab
    std::string word = "abaab";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 5);
  }

  {
    std::string regex = "ba.1.1+*ba.a.b.."; // (ba1 + 1)*baab
    std::string word = "abaab";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 5);
  }

  {
    std::string regex = "a*";
    std::string word = "aaaaaa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 6);
  }
}

TEST_F(AnswerTest, TestFromCondition) {
  {
    std::string regex = "ab+c.aba.*.bac.+.+*"; // ((a+b)c+a(ba)*(b+ac))*
    std::string word = "a";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 1);
  }

  {
    std::string regex = "ab+c.aba.*.bac.+.+*";
    std::string word = "babc";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 3);
  }

  {
    std::string regex = "acb..bab.c.*.ab.ba.+.+*a.";
    std::string word = "abbaa";

    EXPECT_EQ(MaxSubwordInLanguage(regex, word), 5);
  }
}