include altibase_env.mk

INCLUDES=$(IDROPT)${ALTIBASE_HOME}/include
LIBDIR=$(LDROPT)${ALTIBASE_HOME}/lib

all: bmt_insert bmt_select 


bmt_insert_merged.c: bmt_header.c bmt_insert.c bmt_footer.c
	cat bmt_header.c bmt_insert.c bmt_footer.c >bmt_insert_merged.c

bmt_insert.o: bmt_insert_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_insert.o bmt_insert_merged.c

bmt_insert: bmt_insert.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_insert bmt_insert.o -l shardcli $(LIBS)

bmt_select_merged.c: bmt_header.c bmt_select.c bmt_footer.c
	cat bmt_header.c bmt_select.c bmt_footer.c >bmt_select_merged.c

bmt_select.o: bmt_select_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_select.o bmt_select_merged.c

bmt_select: bmt_select.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_select bmt_select.o -lshardcli $(LIBS)


bmt_update_merged.c: bmt_header.c bmt_update.c bmt_footer.c
	cat bmt_header.c bmt_update.c bmt_footer.c >bmt_update_merged.c

bmt_update.o: bmt_update_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_update.o bmt_update_merged.c

bmt_update: bmt_update.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_update bmt_update.o -lshardcli $(LIBS)


bmt_delete_merged.c: bmt_header.c bmt_delete.c bmt_footer.c
	cat bmt_header.c bmt_delete.c bmt_footer.c >bmt_delete_merged.c

bmt_delete.o: bmt_delete_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_delete.o bmt_delete_merged.c

bmt_delete: bmt_delete.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_delete bmt_delete.o -lshardcli $(LIBS)


clean:
	-rm -f bmt_insert bmt_insert.o bmt_insert_merged.c
	-rm -f bmt_select bmt_select.o bmt_select_merged.c
	-rm -f bmt_update bmt_update.o bmt_update_merged.c
	-rm -f bmt_delete bmt_delete.o bmt_delete_merged.c
