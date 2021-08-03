from numpy import log10


class FT:

    '''
    Logarithmic tick format for matplotlib. Change M_x,M_y to change the format fo the ticks.

    _M_xticks,_M_yticks: lists of major ticks
    _M_xticks_exception,_M_yticks_exception: lists of major ticks that will not be labeled
    _m_xticks,_m_yticks:lists of minor ticks
    '''

    def __init__(self,_M_xticks,_M_yticks,_M_xticks_exception,_M_yticks_exception,_m_xticks,_m_yticks,
                     xmin='',xmax='',ymin='',ymax='',xscale='log',yscale='log'):
        self._M_xticks=_M_xticks
        self._M_yticks=_M_yticks
        self._M_xticks_exception=_M_xticks_exception
        self._M_yticks_exception=_M_yticks_exception
        self._m_xticks=_m_xticks
        self._m_yticks=_m_yticks
        self.xmin=xmin
        self.xmax=xmax
        self.ymin=ymin
        self.ymax=ymax
        self.xscale=xscale
        self.yscale=yscale


    def format_ticks(self,plt,sub):
        '''
        plt: matplotlib.pyplot (usually we call 'import matplotlib.pyplot as plt')
        sub: the subplot (assumed  something like sub = fig.add_subplot...)
        '''
        
        _M_xticks=self._M_xticks
        _M_yticks=self._M_yticks
        _M_xticks_exception=self._M_xticks_exception
        _M_yticks_exception=self._M_yticks_exception
        _m_xticks=self._m_xticks
        _m_yticks=self._m_yticks
        xmin=self.xmin
        xmax=self.xmax
        ymin=self.ymin
        ymax=self.ymax
        xscale=self.xscale
        yscale=self.yscale
        
        
        sub.set_xscale(xscale)
        sub.set_yscale(yscale)

        #these have to be here!
        #if you don't change all limits, I will do it automatically (min and max of _M_xticks and _M_yticks)
        if not '' in [xmin,xmax,ymin,ymax]:
            sub.set_xlim(xmin,xmax)
            sub.set_ylim(ymin,ymax)
        else:
            print('Empty axis limits. Using automatic ones.')
            sub.set_xlim(min(_M_xticks),max(_M_xticks))
            sub.set_ylim(min(_M_yticks),max(_M_yticks))

        #set functions that format the ticks
        #value is the number on the axis, tick_number the number of the corresponding tick
        if xscale=='log':
            def M_x(value, tick_number):
                if value in _M_xticks_exception:
                    return ''

                if value==1:
                    return '$1$'

                if value==10:
                    return '$10$'

                if value==0:
                    return '$0$'

                if value!=0:
                    exp=int(log10(value))
                    _v=value/10**exp
                    if round(_v,2)==1.:
                        return '$10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '${0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
        if xscale=='symlog':
            def M_x(value, tick_number):

                
                if value in _M_xticks_exception:
                    return ''
                if value==1:
                    return '$1$'
                if value==-1:
                    return '$-1$'
                if value==10:
                    return '$10$'
                if value==-10:
                    return '$-10$'
                if value==0:
                    return '$0$'
                if value!=0 and value>0:
                    exp=int(log10(value))
                    _v=value/10**exp
                    if round(_v,2)==1.:
                        return '$10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '${0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
                if value!=0 and value<0:
                    exp=int(log10(-value))
                    _v=-value/10**exp
                    if round(_v,2)==1.:
                        return '$-10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '$-{0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
                    
                    
        if xscale=='linear':
            def M_x(value, tick_number):
                if int(value)==float(value):
                    return '${}$'.format(int(value))
                else:
                    return '${}$'.format(value)

                

        if yscale=='log':
            def M_y(value, tick_number):
                if value in _M_yticks_exception:
                    return ''
                if value==1:
                    return '$1$'

                if value==10:
                    return '$10$'

                if value==0:
                    return '$0$'

                if value!=0:
                    exp=int(log10(value))
                    _v=value/10**exp
                    if round(_v,2)==1.:
                        return '$10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '${0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
        if yscale=='symlog':
            def M_y(value, tick_number):

                
                if value in _M_yticks_exception:
                    return ''
                if value==1:
                    return '$1$'
                if value==-1:
                    return '$-1$'
                if value==10:
                    return '$10$'
                if value==-10:
                    return '$-10$'
                if value==0:
                    return '$0$'
                if value!=0 and value>0:
                    exp=int(log10(value))
                    _v=value/10**exp
                    if round(_v,2)==1.:
                        return '$10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '${0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
                if value!=0 and value<0:
                    exp=int(log10(-value))
                    _v=-value/10**exp
                    if round(_v,2)==1.:
                        return '$-10^{0}{1}{2}$'.format('{', exp ,'}')
                    else:
                        return  '$-{0}{1}10^{2}{3}{4}$'.format(_v ,r'\times', '{', exp ,'}')
                    
                    
        if yscale=='linear':
            def M_y(value, tick_number):
                if int(value)==float(value):
                    return '${}$'.format(int(value))
                else:
                    return '${}$'.format(value)
        
        
        def _minor(value, tick_number):
            return ''

        #set the major ticks
        sub.xaxis.set_major_locator(plt.FixedLocator(_M_xticks))
        sub.xaxis.set_major_formatter(plt.FuncFormatter(M_x))

        sub.yaxis.set_major_locator(plt.FixedLocator(_M_yticks))
        sub.yaxis.set_major_formatter(plt.FuncFormatter(M_y))

        #set the mionor ticks
        sub.xaxis.set_minor_locator(plt.FixedLocator(_m_xticks))
        sub.xaxis.set_minor_formatter(plt.FuncFormatter(_minor))
        sub.yaxis.set_minor_locator(plt.FixedLocator(_m_yticks))
        sub.yaxis.set_minor_formatter(plt.FuncFormatter(_minor))
