BIN=poker

final: $(BIN)
	@./$(BIN)

$(BIN): game.c hand.c deck.c player.c main.c mc.c game.h hand.h deck.h player.h mc.h
	gcc game.c hand.c deck.c player.c main.c mc.c -o $(BIN)

clean:
	@rm -f $(BIN)
