FROM alpine:latest AS builder
COPY . /app 
WORKDIR /app/Back
RUN apt-get install sqlite3
RUN sqlite3 quotes.db < quotes.sql
RUN gcc -o server server.c dbmanager.c

FROM alpine:latest
COPY --from=builder /app/Back /app/Back
EXPOSE 8080/tcp         
CMD ["/app/server"]     