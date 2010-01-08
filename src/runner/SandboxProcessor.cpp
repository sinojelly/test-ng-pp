
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <testngpp/runner/EnvironmentCleaner.h>
#include <testngpp/runner/SandboxHandler.h>
#include <testngpp/runner/SandboxProcessor.h>

TESTNGPP_NS_START

void
SandboxProcessor::process(ChannelId rChannel, ChannelId wChannel, \
        EnvironmentCleaner* cleaner, SandboxHandler* childHandler)
{
   cleaner->cleanUp();

   ::close(rChannel);
   int channel = ::dup(wChannel);
   ::close(wChannel);

   childHandler->handle(channel);

   ::close(channel);
}

TESTNGPP_NS_END

