#ifndef Linux 
#include <windows.h>
#else
#include <sys/types.h>
int64_t
InterlockedExchangeAdd64(
    int64_t volatile *Addend,
    int64_t Value
    );
inline
int64_t
InterlockedExchangeAdd64(
    int64_t volatile *Addend,
    int64_t Value
    )
{
    return __sync_fetch_and_add(Addend, Value);
}
#endif
#include <stdint.h>
#include <stdio.h>

#define QUIC_STREAM_RECEIVE_CALL_ACTIVE_FLAG            0x8000000000000000

int main(int argc, char *argv[]) {
    uint64_t RecvCompletionLength = 0;
    uint64_t BufferLength = 1024;

    uint64_t OldRecvCompletionLength = InterlockedExchangeAdd64(
        (int64_t*)&RecvCompletionLength,
        QUIC_STREAM_RECEIVE_CALL_ACTIVE_FLAG);
    printf("OldRecvCompletionLength: %lu\n", OldRecvCompletionLength);

    OldRecvCompletionLength = InterlockedExchangeAdd64(
        (int64_t*)&RecvCompletionLength,
        (int64_t)BufferLength);
    if ((OldRecvCompletionLength & QUIC_STREAM_RECEIVE_CALL_ACTIVE_FLAG) != 0) {
        printf("Receive call is active.\n");
    }

    OldRecvCompletionLength = InterlockedExchangeAdd64(
        (int64_t*)&RecvCompletionLength,
        (int64_t)BufferLength);

    OldRecvCompletionLength = RecvCompletionLength;
    uint64_t Old2RecvCompletionLength = InterlockedExchangeAdd64(
        (int64_t*)&RecvCompletionLength,
        (int64_t)BufferLength);
    uint64_t NewRecvCompletionLength = OldRecvCompletionLength & (~QUIC_STREAM_RECEIVE_CALL_ACTIVE_FLAG);
    OldRecvCompletionLength = InterlockedExchangeAdd64(
        (int64_t*)&RecvCompletionLength,
        NewRecvCompletionLength - OldRecvCompletionLength);

    OldRecvCompletionLength = OldRecvCompletionLength & (~QUIC_STREAM_RECEIVE_CALL_ACTIVE_FLAG);
    printf("OldRecvCompletionLength: %lu\n", OldRecvCompletionLength);
    printf("RecvCompletionLength: %lu\n", RecvCompletionLength);

    return 0; // Exit successfully
}