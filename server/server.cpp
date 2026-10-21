#include <iostream>

#include <boost/asio.hpp>
#include <string>



using boost::asio::local::stream_protocol;

int main() {

    std::cout << "Server started \n";


    const std::string socket_path = "/tmp/uds.sock";

    std::remove(socket_path.c_str());




    boost::asio::io_context io_context;


    stream_protocol::acceptor acceptor(
        io_context,
        stream_protocol::endpoint(socket_path)
    );


    while (true) {

        std::cout << "Waiting on \n ";
        stream_protocol::socket socket(io_context);

        acceptor.accept(socket);

    
        // std::string message = "Hello from Server";
        // boost::asio::write(socket, boost::asio::buffer(message));

        try {
            while (true) {
                char buffer[1024];

                std::size_t n = socket.read_some(
                    boost::asio::buffer(buffer)
                );

                std::string message(buffer, n);

                std::cout << "Client: " << message << std::endl;

                // Antwort an Client
                std::string response = "Server received: " + message + "\n";

                boost::asio::write(
                    socket,
                    boost::asio::buffer(response)
                );
            }
        }
        catch (const boost::system::system_error& e) {
            if (e.code() == boost::asio::error::eof) {
                std::cout << "Client disconnected\n";
            } else {
                std::cerr << "Socket error: "
                          << e.what() << '\n';
            }
        }




    }

    

    return 0;
}