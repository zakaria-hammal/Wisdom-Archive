FROM alpine:latest AS builder
RUN apk add build-base sqlite-dev
RUN apk add sqlite
COPY . /app 
WORKDIR /app/Back
RUN sqlite3 quotes.db < quotes.sql
RUN gcc -o server server.c dbmanager.c -lsqlite3

EXPOSE 8080/tcp         
CMD ["./server"]     