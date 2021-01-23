#include "bidirectional_list.h"

#include <gmock/gmock.h>

namespace containers {

class Tester {
 public:
  static void NodeTests() {
    std::string str = "j3j42234dd";
    std::string str2 = "ddff4454566";
    std::string str3 = "jdj775hhssSasa";
    std::string copy = str3;
    auto* node = new BiDirectionalList<std::string>::Node(str);
    EXPECT_EQ(node->value, str);
    EXPECT_EQ(node->prev_ptr_, nullptr);
    EXPECT_EQ(node->next_ptr_, nullptr);
    auto* node2 = new BiDirectionalList<std::string>::Node(str2, node, nullptr);
    EXPECT_EQ(node2->value, str2);
    EXPECT_EQ(node2->prev_ptr_, node);
    EXPECT_EQ(node2->next_ptr_, nullptr);
    auto* node3 =
        new BiDirectionalList<std::string>::Node(std::move(str3), node, node2);
    EXPECT_EQ(str3, "");
    EXPECT_EQ(node3->value, copy);
    EXPECT_EQ(node3->prev_ptr_, node);
    EXPECT_EQ(node3->next_ptr_, node2);

    delete node;
    delete node2;
    delete node3;
  }

  static void BasicConstructorsTests() {
    {
      const BiDirectionalList<int> list;
      EXPECT_EQ(list.Size(), 0);
      EXPECT_EQ(list.IsEmpty(), true);
    }
    {
      std::initializer_list<int> init_list = {0, 1, 2, 3, 4, 5};
      const BiDirectionalList<int> list(init_list);
      EXPECT_EQ(list.ToVector(), std::vector<int>(init_list));
      EXPECT_EQ(list.size_, 6);
      EXPECT_EQ(list.Size(), 6);
      EXPECT_EQ(list.IsEmpty(), false);

      BiDirectionalList<int> list2(list);
      EXPECT_EQ(list2.ToVector(), std::vector<int>(init_list));
      EXPECT_EQ(list2.size_, 6);

      BiDirectionalList<int> list3(std::move(list2));
      EXPECT_EQ(list3.ToVector(), std::vector<int>(init_list));
      EXPECT_EQ(list3.size_, 6);
      EXPECT_EQ(list2.begin_, nullptr);
      EXPECT_EQ(list2.end_, nullptr);
      EXPECT_EQ(list2.size_, 0);
      EXPECT_EQ(list2.ToVector(), std::vector<int>({}));
      EXPECT_EQ(list2.Size(), 0);
      EXPECT_EQ(list2.IsEmpty(), true);
    }
  }

  static void InsertErasePushPopFrontBackTests() {
    {
      BiDirectionalList<int> list;
      int value = 2;
      ASSERT_DEATH(list.InsertAfter(nullptr, 2), ".*");
      ASSERT_DEATH(list.InsertAfter(nullptr, value), ".*");
      ASSERT_DEATH(list.InsertBefore(nullptr, 2), ".*");
      ASSERT_DEATH(list.InsertBefore(nullptr, value), ".*");
    }
    {
      BiDirectionalList<int> list = {0};
      BiDirectionalList<int>::Node* ptr = list.begin_;
      list.InsertBefore(ptr, 1);
      list.InsertAfter(ptr, 2);
      list.InsertBefore(ptr, 3);
      list.InsertAfter(ptr, 4);
      std::vector<int> expect = {1, 3, 0, 4, 2};
      EXPECT_EQ(list.ToVector(), expect);
      BiDirectionalList<int>::Node* nxt_ptr = ptr->next_ptr_;
      list.Erase(ptr);
      expect = {1, 3, 4, 2};
      EXPECT_EQ(list.ToVector(), expect);
      list.Erase(nxt_ptr);
      expect = {1, 3, 2};
      EXPECT_EQ(list.ToVector(), expect);
    }
    {
      BiDirectionalList<int> list = {0};
      BiDirectionalList<int>::Node* ptr = list.begin_;
      int value = 1;
      list.InsertBefore(ptr, value);
      value = 2;
      list.InsertAfter(ptr, value);
      value = 3;
      list.InsertBefore(ptr, value);
      value = 4;
      list.InsertAfter(ptr, value);
      std::vector<int> expect = {1, 3, 0, 4, 2};
      EXPECT_EQ(list.ToVector(), expect);
      BiDirectionalList<int>::Node* nxt_ptr = ptr->next_ptr_;
      list.Erase(ptr);
      expect = {1, 3, 4, 2};
      EXPECT_EQ(list.ToVector(), expect);
      list.Erase(nxt_ptr);
      expect = {1, 3, 2};
      EXPECT_EQ(list.ToVector(), expect);
      list.PopBack();
      expect = {1, 3};
      EXPECT_EQ(list.ToVector(), expect);
      list.PopFront();
      expect = {3};
      EXPECT_EQ(list.ToVector(), expect);
      list.PopBack();
      expect = {};
      EXPECT_EQ(list.ToVector(), expect);
      ASSERT_DEATH(list.PopBack(), ".*");
    }
    {
      BiDirectionalList<int> list = {9, 2, 1, 2, 0, 2};
      list.Clear();
      EXPECT_EQ(list.size_, 0);
      EXPECT_EQ(list.Size(), 0);
      EXPECT_TRUE(list.IsEmpty());
      ASSERT_DEATH(list.PopFront(), ".*");
    }
    {
      BiDirectionalList<int> list = {9, 2, 1};
      list.PushBack(5);
      std::vector<int> expect = {9, 2, 1, 5};
      EXPECT_EQ(list.Size(), 4);
      EXPECT_EQ(list.ToVector(), expect);

      list.PushFront(122);
      expect = {122, 9, 2, 1, 5};
      EXPECT_EQ(list.Size(), 5);
      EXPECT_EQ(list.ToVector(), expect);

      list.PushFront(-1);
      expect = {-1, 122, 9, 2, 1, 5};
      EXPECT_EQ(list.Size(), 6);
      EXPECT_EQ(list.ToVector(), expect);
    }
    {
      BiDirectionalList<std::string> list = {"a", "b", "c"};
      std::string str = "dd";
      list.PushBack(std::move(str));
      std::vector<std::string> expect = {"a", "b", "c", "dd"};
      EXPECT_EQ(list.Size(), 4);
      EXPECT_EQ(list.ToVector(), expect);
      EXPECT_EQ(str, "");

      str = "ww";
      list.PushFront(std::move(str));
      expect = {"ww", "a", "b", "c", "dd"};
      EXPECT_EQ(list.Size(), 5);
      EXPECT_EQ(list.ToVector(), expect);
      EXPECT_EQ(str, "");

      EXPECT_EQ((*(list.Back())).value, "dd");
      EXPECT_EQ((*(list.Front())).value, "ww");
      list.Clear();
      ASSERT_DEATH(list.Back(), ".*");
      ASSERT_DEATH(list.Front(), ".*");
    }
  }

  static void FindTests() {
    BiDirectionalList<int> list = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 3, 10};
    EXPECT_EQ(list.Find(3), 2);
    EXPECT_EQ(list.Find(9), 8);
    EXPECT_EQ(list.Find(1), 0);
    EXPECT_EQ(list.Find(10), 13);
    EXPECT_EQ(list.Find(0), -1);
    EXPECT_EQ(list.FindAll(1), std::vector<int>({0, 9}));
    EXPECT_EQ(list.FindAll(5), std::vector<int>({4}));
    EXPECT_EQ(list.FindAll(3), std::vector<int>({2, 11, 12}));
    EXPECT_EQ(list.FindAll(0), std::vector<int>({}));
  };

  static void OperatorsTests() {
    {
      BiDirectionalList<std::string> list1({"aa", "bb", "cc"});
      BiDirectionalList<std::string> list2({"dd", "ee", "ff"});
      EXPECT_EQ((list1 = list2).ToVector(),
                std::vector<std::string>({"dd", "ee", "ff"}));
    }

    {
      BiDirectionalList<std::string> list1({"aa", "bb", "cc"});
      BiDirectionalList<std::string> list2({"dd", "ee", "ff"});
      EXPECT_EQ((list1 = std::move(list2)).ToVector(),
                std::vector<std::string>({"dd", "ee", "ff"}));
      EXPECT_EQ(list2.size_, 0);
      EXPECT_EQ(list2.begin_, nullptr);
      EXPECT_EQ(list2.end_, nullptr);
    }
    {
      BiDirectionalList<std::string> list1({"aa", "bb", "cc"});
      BiDirectionalList<std::string> list2({"dd", "ee", "ff"});
      EXPECT_TRUE(list1 != list2);
      EXPECT_FALSE(list1 == list2);
    }
    {
      BiDirectionalList<std::string> list1({"aa", "bb", "cc"});
      BiDirectionalList<std::string> list2({"aa", "bb", "cc"});
      EXPECT_TRUE(list1 == list2);
      EXPECT_FALSE(list1 != list2);
    }
    {
      BiDirectionalList<std::string> list1;
      BiDirectionalList<std::string> list2;
      EXPECT_TRUE(list1 == list2);
      EXPECT_FALSE(list1 != list2);
    }
    {
      BiDirectionalList<std::string> list({"aa", "bb", "cc"});
      EXPECT_EQ((*list[0]).value, "aa");
      EXPECT_EQ((*list[2]).value, "cc");
      ASSERT_DEATH(list[-1], ".*");
      ASSERT_DEATH(list[3], ".*");
    }
  }
};

TEST(BiDirectionalList, NodeTests) {
  Tester::NodeTests();
}

TEST(BiDirectionalList, BasicConstructorsTests) {
  Tester::BasicConstructorsTests();
}

TEST(BiDirectionalList, InsertErasePushPopFrontBackTests) {
  Tester::InsertErasePushPopFrontBackTests();
}

TEST(BiDirectionalList, FindTests) {
  Tester::FindTests();
}

TEST(BiDirectionalList, OperatorsTests) {
  Tester::OperatorsTests();
}

}
