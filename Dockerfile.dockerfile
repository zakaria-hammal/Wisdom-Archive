# Stage 1: Build the server binary
FROM alpine:latest AS builder
RUN apk add build-base sqlite3  # Installs gcc, libc-dev, and make
COPY . /app             # Copy your C code (e.g., server.c)
WORKDIR /app/Back
RUN sqlite3 quotes.db < quotes.sql
RUN gcc -o server server.c dbmanager.c # Compile the server

# Stage 2: Create a minimal runtime image
FROM alpine:latest
COPY --from=builder /app/Back /app/Back
EXPOSE 8080/tcp         # Declare the port your server listens on
CMD ["/app/server"]     # Run the compiled binary