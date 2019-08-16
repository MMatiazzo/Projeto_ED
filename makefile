TARGET   = siguel

PFLAGS   =

CC       = gcc

CFLAGS   = -fstack-protector-all -std=c99 $(PFLAGS) -g

LINKER   = gcc

LFLAGS   = -lm

SRCDIR   = ./
OBJDIR   = ./
BINDIR   = ./

SOURCES  := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/**/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h $(SRCDIR)/**/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -rf

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
