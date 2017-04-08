SRC=.
TGT=.

CXXFLAGS = -Wall -std=c++11
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

$(TGT)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TGT)/out: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm *.o out || true
