#ifndef __JPSOCKEXCEPTIONS_HPP__
#define __JPSOCKEXCEPTIONS_HPP__

namespace jpCppLibs{
	/**
	 * Class the implements the exceptions of the logger
	 */
	class SocketExceptions: public std::exception{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions(bool showErrno = false) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions( const char* error,bool showErrno = false ) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SocketExceptions(std::string error, bool showErrno = false) throw();
		/**
		 * Function to change error message
		 * @param error Error message
		 */
		void setMsg(const char * error) throw();
		/**
		 * Retrive error reason
		 * @return String with the error
		 */
		const char * what() const throw();
		/**
		 * Class destructor
		 */
		~SocketExceptions() throw();
	private:
		/**
		 * Error Message
		 */
		std::string myerror;
		/**
		 * Indicates if errno should be shown or not
		 */
		bool showErrno;
	};

	/**
	 * Generalization classes with the Exceptions
	 */
	class JPNoSocketCreated: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPNoSocketCreated(bool showErrno = false) throw():SocketExceptions("Socket was not created",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPGenericSocket: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPGenericSocket(bool showErrno = false) throw():SocketExceptions("Generic error",showErrno){};
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPGenericSocket( const char* error,bool showErrno = false ) throw():SocketExceptions(error,showErrno){};
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPGenericSocket(std::string error, bool showErrno = false) throw():SocketExceptions(error,showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPInvHostname: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPInvHostname(bool showErrno = false) throw():SocketExceptions("Invalid Hostname",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPInvIpAddress: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPInvIpAddress(bool showErrno = false) throw():SocketExceptions("Invalid Ip Address",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPErrSend: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPErrSend(bool showErrno = false) throw():SocketExceptions("Error sending message",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPErrEmptySocket: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPErrEmptySocket(bool showErrno = false) throw():SocketExceptions("Socket as no data",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPInvSocket: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPInvSocket(bool showErrno = false) throw():SocketExceptions("Invalid socket",showErrno){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPNoConn: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPNoConn() throw():SocketExceptions("A connection must be stablished first, use listen of connect method",false){};
	};
	/**
	 * Generalization classes with the Exceptions
	 */
	class JPDisconnected: public SocketExceptions{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		JPDisconnected() throw():SocketExceptions("Other end disconnected",false){};
	};

};
#endif
