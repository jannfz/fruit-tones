# ==== ESP Config ====
ESP_BOARD_FQBN     = esp8266:esp8266:nodemcu
ESP_PORT           = /dev/tty.usbserial-0001
ESP_SKETCH         = esp
ESP_SRC_DIR        = .
ESP_BUILD_DIR      = build/esp
ESP_MONITOR_BAUD   = 115200

# ==== UNO Config ====
UNO_BOARD_FQBN     = arduino:avr:uno
UNO_PORT           = /dev/tty.usbserial-0002
UNO_SKETCH         = uno
UNO_SRC_DIR        = .
UNO_BUILD_DIR      = build/uno
UNO_MONITOR_BAUD   = 9600

# ==== Shared ====
HTML_SRC           = web/index.html
HTML_HEADER        = esp/index_html.h
HTML_VAR           = index_html

# ==== Default target ====
.PHONY: all
all: esp_compile uno_compile

# ==== ESP Targets ====
.PHONY: esp esp_compile esp_flash esp_monitor
esp: esp_compile

esp_compile: generate_html
	arduino-cli compile \
		--fqbn $(ESP_BOARD_FQBN) \
		--output-dir $(ESP_BUILD_DIR) \
		$(ESP_SKETCH)

esp_flash: esp_compile
	arduino-cli upload \
		-p $(ESP_PORT) \
		--fqbn $(ESP_BOARD_FQBN) \
		--input-dir $(ESP_BUILD_DIR) \
		$(ESP_SKETCH)

esp_monitor:
	picocom $(ESP_PORT) -b $(ESP_MONITOR_BAUD)  --imap lfcrlf
 
# ==== UNO Targets ====
.PHONY: uno uno_compile uno_flash uno_monitor
uno: uno_compile

uno_compile:
	arduino-cli compile \
		--fqbn $(UNO_BOARD_FQBN) \
		--output-dir $(UNO_BUILD_DIR) \
		$(UNO_SKETCH)

uno_flash: uno_compile
	arduino-cli upload \
		-p $(UNO_PORT) \
		--fqbn $(UNO_BOARD_FQBN) \
		--input-dir $(UNO_BUILD_DIR) \
		$(UNO_SKETCH)

uno_monitor:
	picocom $(UNO_PORT) -b $(UNO_MONITOR_BAUD) 

# ==== HTML conversion ====
.PHONY: generate_html
generate_html:
	./html_to_header.sh $(HTML_SRC) $(HTML_HEADER) $(HTML_VAR)

# ==== Cleanup ====
.PHONY: clean
clean:
	rm -rf $(ESP_BUILD_DIR) $(UNO_BUILD_DIR)

