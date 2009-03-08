#
# Compiles the AwS translator
#

include Makefile.common

AWSDIR = aws
SUBDIRS = $(AWSDIR)
PRGOUT = awstranslate
OBJECTS = main.o aws/exception.o aws/parser.o aws/token.o aws/tokenizer.o aws/translator.o
INCLUDE = -Iaws

all:
	@for i in $(SUBDIRS); do \
		( cd $$i ; make ) ; \
	done
	@make $(PRGOUT)

aws:
	@cd $(AWSDIR) ; make

clean:
	@for i in $(SUBDIRS); do \
		(cd $$i ; make clean ) \
	done
	rm -rf *.o

$(PRGOUT): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(PRGOUT) $(OBJECTS)

.PHONY: all clean
