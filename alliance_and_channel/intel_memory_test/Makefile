include altibase_env.mk

INCLUDES=$(IDROPT)${ALTIBASE_HOME}/include
LIBDIR=$(LDROPT)${ALTIBASE_HOME}/lib

all: bmt_insert bmt_select bmt_update bmt_delete


bmt_insert_merged.sc: bmt_header.sc bmt_insert.sc bmt_footer.sc
	cat bmt_header.sc bmt_insert.sc bmt_footer.sc >bmt_insert_merged.sc

bmt_insert_merged.c: bmt_insert_merged.sc
	apre -mt bmt_insert_merged.sc

bmt_insert.o: bmt_insert_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_insert.o bmt_insert_merged.c

bmt_insert: bmt_insert.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_insert bmt_insert.o -lapre -lodbccli $(LIBS)


bmt_select_merged.sc: bmt_header.sc bmt_select.sc bmt_footer.sc
	cat bmt_header.sc bmt_select.sc bmt_footer.sc >bmt_select_merged.sc

bmt_select_merged.c: bmt_select_merged.sc
	apre -mt bmt_select_merged.sc

bmt_select.o: bmt_select_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_select.o bmt_select_merged.c

bmt_select: bmt_select.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_select bmt_select.o -lapre -lodbccli $(LIBS)


bmt_update_merged.sc: bmt_header.sc bmt_update.sc bmt_footer.sc
	cat bmt_header.sc bmt_update.sc bmt_footer.sc >bmt_update_merged.sc

bmt_update_merged.c: bmt_update_merged.sc
	apre -mt bmt_update_merged.sc

bmt_update.o: bmt_update_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_update.o bmt_update_merged.c

bmt_update: bmt_update.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_update bmt_update.o -lapre -lodbccli $(LIBS)


bmt_delete_merged.sc: bmt_header.sc bmt_delete.sc bmt_footer.sc
	cat bmt_header.sc bmt_delete.sc bmt_footer.sc >bmt_delete_merged.sc

bmt_delete_merged.c: bmt_delete_merged.sc
	apre -mt bmt_delete_merged.sc

bmt_delete.o: bmt_delete_merged.c
	$(COMPILE.c) $(INCLUDES) $(CCOUT) bmt_delete.o bmt_delete_merged.c

bmt_delete: bmt_delete.o
	$(LD) $(LFLAGS) $(LIBDIR) -o bmt_delete bmt_delete.o -lapre -lodbccli $(LIBS)


clean:
	-rm -f bmt_insert bmt_insert.o bmt_insert_merged.c bmt_insert_merged.sc
	-rm -f bmt_select bmt_select.o bmt_select_merged.c bmt_select_merged.sc
	-rm -f bmt_update bmt_update.o bmt_update_merged.c bmt_update_merged.sc
	-rm -f bmt_delete bmt_delete.o bmt_delete_merged.c bmt_delete_merged.sc
