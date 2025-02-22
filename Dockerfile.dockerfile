FROM alpine:latest AS builder
RUN apk add build-base sqlite3  # Installs gcc, libc-dev, and make
COPY . /app 
WORKDIR /app/Back
RUN sqlite3 quotes.db < quotes.sql
RUN gcc -o server server.c dbmanager.c # Compile the server

FROM alpine:latest
COPY --from=builder /app/Back /app/Back
EXPOSE 8080/tcp         
CMD ["/app/server"]     