rocksdbDIR=/mnt/sdb/facebook_rocksdb
include $(rocksdbDIR)/make_config.mk

.PHONY: clean

ifndef DISABLE_JEMALLOC
	ifdef JEMALLOC
		PLATFORM_CXXFLAGS += -DROCKSDB_JEMALLOC -DJEMALLOC_NO_DEMANGLE
	endif
	EXEC_LDFLAGS := $(JEMALLOC_LIB) $(EXEC_LDFLAGS) -lpthread
	PLATFORM_CXXFLAGS += $(JEMALLOC_INCLUDE)
endif

ifneq ($(USE_RTTI), 1)
	CXXFLAGS += -fno-rtti
endif

cli: cli.o
	$(CXX) $@.o -o$@ $(rocksdbDIR)/librocksdb.a $(PLATFORM_LDFLAGS) $(EXEC_LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I$(rocksdbDIR)/include

clean:
	rm cli *.o

