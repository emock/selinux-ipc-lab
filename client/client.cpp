#include <iostream>

#include <boost/asio.hpp>


using boost::asio::local::stream_protocol;


int main() {

    // std::cout << "Hello from Client\n";


    boost::asio::io_context io_context;

    stream_protocol::socket socket(io_context);

    socket.connect(stream_protocol::endpoint("/tmp/uds.sock"));


    std::string message = "44";

    boost::asio::write(socket, boost::asio::buffer(message));



    char buffer[1024];

    


    std::size_t length = socket.read_some(boost::asio::buffer(buffer));

    std::cout.write(buffer, length);

    return 0;

}