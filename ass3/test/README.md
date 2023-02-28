# Testing Rationale
There are 7 parts of the testing.
> + graph_test_accessors.cpp
>>This should be the first part to be tested as I use the accessors to implement the other parts of the graph. In this section, the values that return true or false and the values that will raise error messages are tested.

> + graph_test_constructors.cpp
>> In this section, the default constructor and constructors taking different value are tested. And move constructor, move assignment, copy constuctor, copy assignment are also tested here.

> + graph_test_modifiers.cpp
>> In this section, inserting or erasing nodes or edges, merging and replacing nodes, replacing nodes and clear the graph are tested. Both the legal values and the illegal values are tested.
>> The duplicate case is including while testing the merge_and_replace_node part.
>> I use is_node and is_erase(help function written by myself but not in the spec) to check whether the inserting or erasing works.

> + graph_test_iterator_access.cpp
>>In this section, I check whether the g.begin() and g.end() represent the head and tail of the graph g. I use find() function to find the expected position and compare the expected position and testing position.

> + graph_test_comparisons.cpp
>>In this section, I insert the same nodes and edges to 2 different graphs and check whether they are equal.

> + graph_test_extractor.cpp
>>In this section, I insert the nodes and edges provided of the example in the specs. And checking the whether the output is same as that offered by the tutor on ed.

> + graph_test_iterator.cpp
>> In this section, I test the iterator. In the traversal part, the prefix and postfix ++/-- are tested separately.
