ADD_EXECUTABLE(example1 example1.cpp)
TARGET_LINK_LIBRARIES(example1 ${ATOMSPACE_LIBRARIES} ${COGUTIL_LIBRARY})
ADD_EXECUTABLE(example2 example2.cpp)
TARGET_LINK_LIBRARIES(example2 ${ATOMSPACE_LIBRARIES} ${COGUTIL_LIBRARY})
ADD_EXECUTABLE(example3 example3.cpp)
TARGET_LINK_LIBRARIES(example3 ${ATOMSPACE_LIBRARIES} ${COGUTIL_LIBRARY})
ADD_EXECUTABLE(example4 example4.cpp)
TARGET_LINK_LIBRARIES(example4 ${ATOMSPACE_LIBRARIES} ${COGUTIL_LIBRARY})
