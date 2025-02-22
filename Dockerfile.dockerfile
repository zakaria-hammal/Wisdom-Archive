FROM alpine:latest AS builder
RUN apk add build-base
COPY . /app 
WORKDIR /app/Back
RUN sqlite3 quotes.db < quotes.sql
RUN gcc -o server server.c dbmanager.c

FROM alpine:latest
COPY --from=builder /app/Back /app/Back
EXPOSE 8080/tcp         
CMD ["/app/server"]     