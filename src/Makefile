CXXFLAGS = -g -O3 -Wall -fmessage-length=0 -std=c++17

# Fill, Reject by No. 2
INPUT1 = \\n\
FBCO, kjohnes, 00001, EQTY, TWAP, AMZN, BUY, 600, 2\\n\
FBCO, kjohnes, 00xx1, EQTY, TWAP, AMZN, BUY, 1401, 2\\n\

# Fill, Reject by No. 4
INPUT2 = \\n\
FBCO, kjohnes, 09999, EQTY, TWAP, AMZN, BUY, 1000, 2\\n\
FBCO, kjohnes, 09999, EQTY, VWAP, AMZN, BUY, 251, 2\\n\


OBJS = order_limit_check_main.o
LIBS =
INCLUDES = -I.

TARGET = order_limit_check_main

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(INCLUDES)

test:
	echo  "$(INPUT1)" | ./order_limit_check_main -f order_limits.csv
	echo  "$(INPUT2)" | ./order_limit_check_main -f order_limits.csv

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

