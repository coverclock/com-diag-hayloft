/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_HAYLOFT_PACKET_H_
#define _COM_DIAG_HAYLOFT_PACKET_H_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <cstring>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/InputOutput.h"

namespace com {
namespace diag {
namespace hayloft {

/**
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketData {

public:

	static const size_t PREPEND = 1;

	static const size_t EITHER = 2;

	static const size_t APPEND = unsignedintmaxof(size_t);

	typedef uint8_t Datum;

	explicit PacketData(void * data /* UNTAKEN */, size_t ve, size_t vf = EITHER)
	: next(0)
	, payload(static_cast<Datum*>(data))
	, head(payload)
	, tail(payload + ve)
	, extent(ve)
	, fraction(vf)
	{}

	virtual ~PacketData() {}

	size_t append(const void * data /* COPIED */, size_t length);

	size_t prepend(const void * data /* COPIED */, size_t length);

	size_t consume(void * data, size_t length);

	size_t consume(size_t length) { return consume(0, length); }

	/**
	 * Points to the next PacketData in a linked list.
	 */
	PacketData * next;

protected:

	/**
	 * Points to the first octet of the buffer.
	 * Never altered after construction.
	 */
	Datum * const payload;

private:

	/**
	 * The size of the buffer in octets.
	 * Never altered after construction.
	 */
	const size_t extent;

	/**
	 * The amount of slack to leave at the beginning of the buffer when the
	 * first octet is appended or prepended in terms of a fraction of the
	 * remaining unused space.
	 * Never altered after construction.
	 * (>remaining): leave no the space at the beginning (default).
	 * (2): place the first appended or prepended data smack in the middle.
	 * (1): leave all space at the beginning.
	 * (0): wackiness ensues.
	 */
	const size_t fraction;

	/**
	 * Points to the first used octet in the buffer.
	 * (head==0): the buffer is empty; otherwise
	 * (head-payload): number of unused octets at the front of the buffer.
	 * (head==payload): no unused octets at the front of the buffer.
	 * (tail-head): the number of used octets in the buffer.
	 */
	Datum * head;

	/**
	 * Points to the first unused octet just past last used octet in the buffer.
	 * (tail==0): the buffer is empty; otherwise
	 * (payload+extent-tail): number of unused octets at the rear of the buffer.
	 * ((payload+extent)==tail): no unused octets at the rear of the buffer.
	 * (tail-head): the number of used octets in the buffer.
	 */
	Datum * tail;

public:

	bool empty() const { return (head == 0); }

	size_t length() const { return extent; }

	const void * buffer() const { return head; }

	size_t size() const { return tail - head; }

	size_t prefix() const { return (head == 0) ? extent : head - payload; }

	size_t suffix() const { return (tail == 0) ? extent : payload + extent - tail; }

	void clear() { head = 0; tail = 0; }

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
    PacketData(const PacketData& that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
    PacketData& operator=(const PacketData& that);

};

/**
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketDataDynamic : public PacketData {

public:

	using PacketData::PREPEND;

	using PacketData::EITHER;

	using PacketData::APPEND;

	using PacketData::Datum;

	explicit PacketDataDynamic(Datum * data /* TAKEN */, size_t ve, size_t vf = EITHER)
	: PacketData(data, ve, vf)
	{}

	virtual ~PacketDataDynamic() { delete [] payload; }

	using PacketData::append;

	using PacketData::prepend;

	using PacketData::consume;

	using PacketData::next;

	using PacketData::empty;

	using PacketData::length;

	using PacketData::buffer;

	using PacketData::size;

	using PacketData::clear;

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	PacketDataDynamic(const PacketDataDynamic & that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	PacketDataDynamic& operator=(const PacketDataDynamic & that);

};

/**
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketBuffer : public PacketData {

public:

	using PacketData::PREPEND;

	using PacketData::EITHER;

	using PacketData::APPEND;

	using PacketData::Datum;

	explicit PacketBuffer(void * buffer /* UNTAKEN */, size_t ve, size_t vf = EITHER)
	: PacketData(buffer, ve, vf)
	{ clear(); }

	virtual ~PacketBuffer() {}

	using PacketData::append;

	using PacketData::prepend;

	using PacketData::consume;

	using PacketData::next;

	using PacketData::empty;

	using PacketData::length;

	using PacketData::buffer;

	using PacketData::size;

	using PacketData::prefix;

	using PacketData::suffix;

	using PacketData::clear;

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	PacketBuffer(const PacketBuffer & that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	PacketBuffer& operator=(const PacketBuffer & that);

};

/**
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketBufferDynamic : public PacketBuffer {

public:

	using PacketBuffer::PREPEND;

	using PacketBuffer::EITHER;

	using PacketBuffer::APPEND;

	using PacketBuffer::Datum;

	explicit PacketBufferDynamic(Datum * buffer /* TAKEN */, size_t ve, size_t vf = EITHER)
	: PacketBuffer(buffer, ve, vf)
	{}

	explicit PacketBufferDynamic(size_t ve, size_t vf = EITHER)
	: PacketBuffer(new Datum[ve], ve, vf)
	{}

	virtual ~PacketBufferDynamic() { delete [] payload; }

	using PacketBuffer::append;

	using PacketBuffer::prepend;

	using PacketBuffer::consume;

	using PacketBuffer::next;

	using PacketBuffer::empty;

	using PacketBuffer::length;

	using PacketBuffer::buffer;

	using PacketBuffer::size;

	using PacketBuffer::prefix;

	using PacketBuffer::suffix;

	using PacketBuffer::clear;

private:

    /**
     *  Copy constructor.
     *  @param that refers to an R-value object of this type.
     */
	PacketBufferDynamic(const PacketBufferDynamic & that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
	PacketBufferDynamic& operator=(const PacketBufferDynamic & that);

};

class Packet;

/**
 * Implements an Input functor for a Packet.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketInput : public com::diag::desperado::Input {

public:

    /**
     * Ctor.
     * @param rs refers to the Packet for this functor.
     */
    explicit PacketInput(Packet & rp /* UNTAKEN */)
    : packet(rp)
    {}

    /**
     * Dtor.
     */
    virtual ~PacketInput();

    /**
     * Returns the next character.
     * @return a character in an integer if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     * Pushes an character in an integer back to the input to be
     * returned on the next call to the input character functor.
     * Only one pushed back character is guaranteed to succeed in
     * between input character operations. The character
     * pushed back does not have to be the previous character input,
     * or even any character that was ever input, although at least
     * one character must have been previously input.
     * @param ch is the character to push back into the input.
     * @return the pushed back character is successful, EOF otherwise.
     */
    virtual int operator() (int ch);

    /**
     * Inputs a newline or NUL terminated line into the buffer of
     * the specified size. If a newline is encountered, it is input
     * into the buffer. Guarantees that the buffer is NUL terminated
     * if it is at least one octet in size. Guarantees that no more
     * than the specified number of octets are returned.
     * @param bufferpoints to the buffer.
     * @param size is the size of the buffer in octets. Size should be no
     * larger than the largest possible signed integer.
     * @return the number of octets input (which may be zero) including the
     * terminating NUL, if successful, EOF otherwise.
     */
    virtual ssize_t operator() (char * buffer, size_t size);

    /**
     * Inputs binary data into a buffer. The intent is that the
     * operation blocks until the minimum number of requested octets
     * are input or EOF or an error occurs. If more octets are available
     * without blocking, up to the maximum number of requested octets
     * are input. Specific implementations may differ. The interface
     * only guarantees that no more than the maximum number of octets
     * a input into the buffer. The functor does not NUL terminate the buffer.
     * @param buffer  points to the buffer.
     * @param minimum is the minimum number of octets to input.
     * @param maximum is the maximum number of octets to input.
     * @return the number of octets input (which may be any number less
     * than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (void * buffer, size_t minimum, size_t maximum);

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     * @param level sets the verbosity of the output. What this means
     * is object dependent. However, the level is passed from outer to inner
     * objects this object calls the show methods of its inherited or composited
     * objects.
     * @param display  points to the output object to which output is sent. If
     * null (zero), the default platform output object is used as the effective
     * output object. The effective output object is passed from outer to inner
     * objects as this object calls the show methods of its inherited and
     * composited objects.
     * @param indent specifies the level of indentation. One more than this
     * value is passed from outer to inner objects as this object calls the
     * show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, Output * display = 0, int indent = 0) const;

private:

    Packet& packet; /* UNTAKEN */

};

/**
 * Implements an Output functor for a Packet.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketOutput : public com::diag::desperado::Output {

public:

    /**
     * Ctor.
     *@param rs refers to the Packet for this functor.
     */
    explicit PacketOutput(Packet & rp /* UNTAKEN */)
    : packet(rp)
    {}

    /**
     * Dtor.
     */
    virtual ~PacketOutput() { (*this)(); }

    /**
     * Outputs a character in integer form.
     * @param c is a character in integer form.
     * @return the output character if successful, EOF otherwise.
     */
    virtual int operator() (int c);

    /**
     * Outputs a string of no more than the specified length not including
     * its terminating NUL.
     * @param s points to a constant NUL-terminated string.
     * @param size is the size of the string in octets.
     * @return the number of octets output if successful (which may be zero),
     * EOF otherwise.
     */
    virtual ssize_t operator() (const char * s /* COPIED */, size_t size = com::diag::desperado::Output::maximum_string_length);

    /**
     * Format a variable length argument list and output the result.
     * @paramformatis a NUL-terminated string containing a printf-style format
     * statement.
     * @param ap is a variable length argument object.
     * @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const char * format, va_list ap);

    /**
     * Outputs binary data from a buffer. The intent is that the
     * operation blocks until the minimum number of requested octets
     * are output or EOF or an error occurs. If more space is available
     * without blocking, up to the maximum number of requested octets
     * may be output. Specific implementations may differ. The interface
     * only guarantees that at least the minimum number of octets are
     * output from the buffer.
     * @param buffer points to the buffer.
     * @param minimum is the minimum number of octets to output.
     * @param maximum is the maximum number of octets to output.
     * @return the number of octets output (which may be any number less
     * than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const void * buffer /* COPIED */, size_t minimum, size_t maximum);

    /**
     * Flush any buffered data to the file. Specific implementations may do
     * nothing if they do not buffer.
      * @return a non-negative number if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     * @param level sets the verbosity of the output. What this means is object
     * dependent. However, the level is passed from outer to inner objects this
     * object calls the show methods of its inherited or composited objects.
     * @param display points to the output object to which output is sent. If
     * null (zero), the default platform output object is used as the effective
     * output object. The effective output object is passed from outer to
     * inner objects as this object calls the show methods of its inherited and
     * composited objects.
     * @param indent specifies the level of indentation. One more than this
     * value is passed from outer to inner objects as this object calls the
     * show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, Output * display = 0, int indent = 0) const;

private:

    Packet& packet;

};

/**
 * Implements a linked list of buffers to which data can be prepended and
 * appended. Data can be appended using an Input functor and consumed using
 * an Output functor.
 * @author coverclock@diag.com (Chip Overclock)
 */
class Packet : public com::diag::desperado::InputOutput {

public:

	static const size_t ALLOCATION = 1024;

    /**
     *  Ctor.
     *  @param va is the default allocation size in octets.
     */
    explicit Packet(size_t va = ALLOCATION)
    : allocation(va)
    , head(0)
    , tail(0)
    //, in(*this)
    //, out(*this)
    {}

    /**
     *  Destructor.
     */
    virtual ~Packet() { clear(); }

    /**
     *  Returns a reference to the input functor interface.
     *  @return a reference to the input functor interface.
     */
    //virtual Input& input() { return in; }

    /**
     *  Returns a reference to the output functor interface.
     *  @return a reference to the output functor interface.
     */
    //virtual Output& output() { return out; }

    bool empty() const { return (head == 0); }

    void clear();

	void append(PacketData & rd /* TAKEN */);

	void prepend(PacketData & rd /* TAKEN */);

	size_t append(const void * data /* COPIED */, size_t length);

	size_t prepend(const void * data /* COPIED */, size_t length);

	size_t consume(void * data, size_t length);

	size_t consume(size_t length) { return consume(0, length); }

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     * @param level sets the verbosity of the output. What this means is object
     * dependent. However, the level is passed from outer to inner objects this
     * object calls the show methods of its inherited or composited objects.
     * @param display points to the output object to which output is sent. If
     * null (zero), the default platform output object is used as the effective
     * output object. The effective output object is passed from outer to
     * inner objects as this object calls the show methods of its inherited and
     * composited objects.
     * @param indent specifies the level of indentation. One more than this
     * value is passed from outer to inner objects as this object calls the
     * show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, Output * display = 0, int indent = 0) const;

private:

    size_t allocation;

    PacketData * head;

    PacketData * tail;

    /**
     *  This is the Input functor to the Packet.
     */
    //PacketInput in;

    /**
     *  This is the Output functor to the Packet.
     */
    //PacketOutput out;

private:

    /**
     *  Copy ctor.
     *  @param that refers to an R-value object of this type.
     */
    Packet(const Packet & that);

    /**
     *  Assignment operator.
     *  @param that refers to an R-value object of this type.
     */
    Packet& operator=(const Packet & that);

};

}
}
}

#endif
