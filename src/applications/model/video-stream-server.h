/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef VIDEO_STREAM_SERVER_H
#define VIDEO_STREAM_SERVER_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/string.h"
#include "ns3/ipv4-address.h"
#include "ns3/traced-callback.h"

#include <fstream>
#include <unordered_map>
namespace ns3 {

class Socket;
class Packet;

  /**
   * @brief A Video Stream Server
   */
  class VideoStreamServer : public Application
  {
  public:
    /**
     * @brief Get the type ID.
     * 
     * @return the object TypeId
     */
    static TypeId GetTypeId (void);

    VideoStreamServer ();

    virtual ~VideoStreamServer ();

    /**
     * @brief Set the name of the file containing the frame sizes.
     * 
      * @param frameFile the file name
     */
    void SetFrameFile (std::string frameFile);

    /**
     * @brief Get the name of the file containing the frame sizes.
     * 
     * @return the file name 
     */
    std::string GetFrameFile (void) const;

    /**
     * @brief Set the maximum packet size.
     * 
     * @param maxPacketSize the largest number of bytes a packet can be
     */
    void SetMaxPacketSize (uint32_t maxPacketSize);

    /**
     * @brief Get the maximum packet size.
     * 
     * @return uint32_t the largest number of bytes a packet can be
     */
    uint32_t GetMaxPacketSize (void) const;

  protected:
    virtual void DoDispose (void);

  private:

    virtual void StartApplication (void);
    virtual void StopApplication (void);

    /**
     * @brief Send the video frame to the given ipv4 address.
     * 
     * @param ipAddress ipv4 address
     */
    void Send (uint32_t ipAddress);

    /**
     * @brief Handle a packet reception.
     * 
     * This function is called by lower layers.
     * 
     * @param socket the socket the packet was received to
     */
    void HandleRead (Ptr<Socket> socket);

    /**
     * @brief The information required for each client.
     */
    typedef struct ClientInfo
    {
      Address m_address; //!< Address
      Ptr<Socket> m_socket; //!< Socket
      uint32_t m_sent; //!< Counter for sent frames
      uint16_t m_videoLevel; //! Video level
    } ClientInfo;

    Time m_interval; //!< Packet inter-send time
    uint32_t m_maxPacketSize; //!< Maximum size of the packet to be sent
    Ptr<Socket> m_socket; //!< Socket

    uint16_t m_port; //!< The port 
    Address m_local; //!< Local multicast address
    EventId m_sendEvent; //!< Send events for different clients

    uint32_t m_frameRate; //!< Number of frames per second to be sent
    uint32_t m_videoLength; //!< Length of the video in seconds
    std::string m_frameFile; //!< Name of the file containing frame sizes
    std::vector<uint32_t> m_frameSizeList; //!< List of video frame sizes
    
    std::unordered_map<uint32_t, ClientInfo*> m_clients; //!< Information saved for each client
    const uint32_t m_frameSizes[5] = {0, 345600, 921600, 2073600, 2211840}; //!< Frame size for 480p, 720p, 1080p and 2K
  };

} // namespace ns3


#endif /* VIDEO_STREAM_SERVER_H */