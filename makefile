ODIR=obj
SDIR=src
_DEPS = IndependentSetExtender.h IndependentSetScorer.h MaximalIndependentSetsEnumerator.h ResultsHandler.h IndSetExtByTriangulation.h IndSetExtBySeparators.h IndSetScorerByTriangulation.h MinimalSeparatorsEnumerator.h MinimalTriangulationsEnumerator.h GraphReader.h MinimalTriangulator.h Graph.h DataStructures.h SeparatorGraph.h Converter.h SeparatorsScorer.h ChordalGraph.h SubGraph.h SuccinctGraphRepresentation.h
_OBJ = MinTriangulationsEnumeration.o IndSetExtByTriangulation.o IndSetExtBySeparators.o IndSetScorerByTriangulation.o MinimalSeparatorsEnumerator.o MinimalTriangulationsEnumerator.o GraphReader.o MinimalTriangulator.o Graph.o DataStructures.o SeparatorGraph.o Converter.o SeparatorsScorer.o ChordalGraph.o SubGraph.o
 
CC=g++
CFLAGS=-I$(SDIR)
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

enumerator: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)



