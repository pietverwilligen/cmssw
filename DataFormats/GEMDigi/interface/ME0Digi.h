#ifndef GEMDigi_ME0Digi_h
#define GEMDigi_ME0Digi_h

#include <boost/cstdint.hpp>
#include <iosfwd>

class ME0Digi{

public:
  explicit ME0Digi (int strip, float tof, int pdgid, bool prompt);
  ME0Digi ();

  bool operator==(const ME0Digi& digi) const;
  bool operator!=(const ME0Digi& digi) const;
  bool operator<(const ME0Digi& digi) const;

  int strip() const { return strip_; }
  float tof()  const { return tof_;}
  // coding mc-truth
  int pdgid() const { return pdgid_;}
  bool prompt() const { return prompt_;}
  void print() const;

private:
  int strip_;
  float tof_;
  // coding mc-truth
  int pdgid_;
  bool prompt_;
};

std::ostream & operator<<(std::ostream & o, const ME0Digi& digi);

#endif

