#ifndef IPLAYER_H_
#define IPLAYER_H_

class IPlayer
{
public:
   virtual void	UpdateScores(size_t) = 0;
   virtual size_t	GetId() const = 0;
   virtual ~IPlayer() {}
};

#endif /* !IPLAYER_H_ */
