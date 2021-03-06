/*
 *  Copyright (C) 2013 Arne Morten Kvarving
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "EncoderAddon.h"

using namespace ADDON;
using namespace KODI::CDRIP;

CEncoderAddon::CEncoderAddon(const AddonInfoPtr& addonInfo)
  : IAddonInstanceHandler(ADDON_INSTANCE_AUDIOENCODER, addonInfo)
{
  // Create "C" interface structures, used as own parts to prevent API problems on update
  m_struct.toAddon = new KodiToAddonFuncTable_AudioEncoder();
  m_struct.toKodi = new AddonToKodiFuncTable_AudioEncoder();
  m_struct.toKodi->kodiInstance = this;
  m_struct.toKodi->write = cb_write;
  m_struct.toKodi->seek = cb_seek;
}

CEncoderAddon::~CEncoderAddon()
{
  // Delete "C" interface structures
  delete m_struct.toAddon;
  delete m_struct.toKodi;
}

bool CEncoderAddon::Init()
{
  if (CreateInstance(&m_struct) != ADDON_STATUS_OK || !m_struct.toAddon->start)
    return false;

  m_addonInstance = m_struct.toAddon->addonInstance;

  KODI_ADDON_AUDIOENCODER_INFO_TAG tag{};
  tag.channels = m_iInChannels;
  tag.samplerate = m_iInSampleRate;
  tag.bits_per_sample = m_iInBitsPerSample;
  tag.track_length = m_iTrackLength;
  tag.title = m_strTitle.c_str();
  tag.artist = m_strArtist.c_str();
  tag.album_artist = m_strAlbumArtist.c_str();
  tag.album = m_strAlbum.c_str();
  tag.release_date = m_strYear.c_str();
  tag.track = atoi(m_strTrack.c_str());
  tag.genre = m_strGenre.c_str();
  tag.comment = m_strComment.c_str();

  return m_struct.toAddon->start(m_addonInstance, &tag);
}

ssize_t CEncoderAddon::Encode(uint8_t* pbtStream, size_t nNumBytesRead)
{
  if (m_struct.toAddon->encode)
    return m_struct.toAddon->encode(m_addonInstance, pbtStream, nNumBytesRead);
  return 0;
}

bool CEncoderAddon::Close()
{
  bool ret = false;
  if (m_struct.toAddon->finish)
    ret = m_struct.toAddon->finish(m_addonInstance);

  DestroyInstance();

  return ret;
}

ssize_t CEncoderAddon::Write(const uint8_t* data, size_t len)
{
  return CEncoder::Write(data, len);
}

ssize_t CEncoderAddon::Seek(ssize_t pos, int whence)
{
  return CEncoder::Seek(pos, whence);
}

ssize_t CEncoderAddon::cb_write(KODI_HANDLE kodiInstance, const uint8_t* data, size_t len)
{
  if (!kodiInstance || !data)
    return -1;
  return static_cast<CEncoderAddon*>(kodiInstance)->Write(data, len);
}

ssize_t CEncoderAddon::cb_seek(KODI_HANDLE kodiInstance, ssize_t pos, int whence)
{
  if (!kodiInstance)
    return -1;
  return static_cast<CEncoderAddon*>(kodiInstance)->Seek(pos, whence);
}
