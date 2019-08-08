# include <sstream>
# include <map>
# include <string>
# include <vector>
# include <queue>
# include <iostream>
# include <fstream>
# include <algorithm>

using namespace std;

class digraphException
{
  public:
    digraphException( const string & msg = "" ) : message( msg )
      { }
    virtual ~digraphException( )
      { }
    virtual string toString( ) const

      { return "Exception " + string( ": " ) + what( ); }

    virtual string what( ) const
      { return message; }

  private:
    string message;
};

template<typename T>
struct Vertex;

// Basic item stored in adjacency list.
template<typename T>
struct Edge
{
    Vertex<T> *dest;   // incident vertex in edge
    T cost;    // Edge cost

    Edge( Vertex<T> *d = 0, T c = 0 )
        : dest( d ), cost( c ) { }
};

// Structure stored in priority queue for Dijkstra's algorithm.
template<typename T>
struct Path
{
    Vertex<T> *dest;   // target vertex
    T cost;    // distance

    Path( Vertex<T> *d = 0, T c = 0 )
        : dest( d ), cost( c ) { }

    bool operator> ( const Path & rhs ) const
        { return cost > rhs.cost; }
    bool operator< ( const Path & rhs ) const
        { return cost < rhs.cost; }
};

// Vertex structure.
template<typename T>
struct Vertex
{
    int number;    // Vertex number
    vector<Edge<T> > adj;   // Adjacent list
    T dist;    // Distance
    Vertex *prev;   // Previous vertex on shortest path
    bool known;    // Flag used in Dijkstra's algorithm

    Vertex( const int & num ) : number( num )
        { reset( ); }

    void reset( )
        { dist = 9999; prev = NULL; known = false; }
};

template<typename T>
struct MapDef
{
    typedef map<int,Vertex<T> *,less<int> > vmap;
};

/**
 * Graph class interface: evaluate shortest paths.
 *
 * CONSTRUCTION: with no parameters.
 *
 ********************PUBLIC OPERATIONS**************************
 * Vertex * getVertex( int & vertexNumber ) --> return a vertex
 * void addEdge( string v, string w, int cvw ) --> Add additional edge
 * string getPath( int destNumber ) --> get path after alg is run
 * void clearAll( ) --> clear members before alg is run
 ************************ERRORS*********************************
 * Some error checking is performed to make sure graph is ok,
 * and to make sure graph satisfies properties needed by
 * algorithm. GraphException is thrown if error is detected.
 */
template<typename T>
class digraph
{
    public:
        digraph( ) { }
        ~digraph( );

        MapDef<int>::vmap vertexMap;

        Vertex<T> * getVertex( const int & vertexNumber );
        void addEdge( const int & sourceNumber,const int & destNumber, int cost );
        string getPath( const int & destNumber ) const;
        void clearAll( );

    private:
        string getPath( const Vertex<T> & dest ) const;

        // Copy semantics are disabled.
        digraph( const digraph & rhs ) { }
        const digraph & operator= ( const digraph & rhs )
            { return *this; }
};

// Destructor: clean up the Vertex objects.
template<typename T>
digraph<T>::~digraph( )
{
    for( MapDef<int>::vmap::iterator itr = vertexMap.begin( ); itr != vertexMap.end( ); ++itr )
        delete (*itr).second;
}

/**
 * If vertexName is not present, add it to vertexMap.
 * In either case, return (a pointer to) the Vertex.
 */
template<typename T>
Vertex<T> * digraph<T>::getVertex( const int &vertexNumber )
{
    MapDef<int>::vmap::iterator itr = vertexMap.find( vertexNumber );

    if( itr == vertexMap.end( ) )
    {
        Vertex<T> *newv = new Vertex<T>( vertexNumber );
        vertexMap[ vertexNumber ] = newv;
        return newv;
    }
    return (*itr).second;
}

// Add a new edge to the graph
template<typename T>
void digraph<T>::addEdge( const int & sourceNumber, const int & destNumber, int cost )
{
    Vertex<T> * v = getVertex( sourceNumber );
    Vertex<T> * w = getVertex( destNumber );
    v->adj.push_back( Edge<T>( w, cost ) );
}

// Initialize the vertex members info prior to running Dijkstra's algorithm
template<typename T>
void digraph<T>::clearAll( )
{
    for( MapDef<int>::vmap::iterator itr = vertexMap.begin( ); itr != vertexMap.end( ); ++itr )
        (*itr).second->reset( );
}

/**
 * Recursive routine to get shortest path to dest after SSAD has run.
 * Assume that the path exists.
 */
template<typename T>
string digraph<T>::getPath( const Vertex<T> & dest ) const
{   
    string str;
    if( dest.prev != NULL ){
        
        str = getPath( *dest.prev ) + "  ";
    }
    stringstream ss;
    ss << dest.number;
    return str += ss.str( );
}

/**
 * Driver routine to handle unreachables and return total cost.
 * It calls recursive routine to print shortest path to
 * destNode after Dijkstra's algorithm has run.
 */
template<typename T>
string digraph<T>::getPath( const int & destNumber ) const
{
    MapDef<int>::vmap::const_iterator itr = vertexMap.find( destNumber );
    if( itr == vertexMap.end( ) )
        throw digraphException( "Destination vertex not found" );

    string str;
    const Vertex<T> & w = *(*itr).second;
    if( w.dist == 9999 )
    {
        stringstream ss;
        ss << destNumber;
        str = ss.str( ) + "\t\tinf";
    }
    else
    {   
        stringstream ss, ssw;
        ss << destNumber;
        ssw << w.dist;
        str = ss.str( ) + "\t\t" + ssw.str( ) + "\t\t" + getPath( w );
    }
    return str += "\n";
}

// Dijkstra's SSAD algorithm.
template<typename T>
void SSAD( digraph<T> & g, const int & startNumber )
{
    priority_queue<Path<T>, vector<Path<T> >, greater<Path<T> > > pq;
    Path<T> vrec;  // Stores the result of a deleteMin

    MapDef<int>::vmap::iterator itr = g.vertexMap.find( startNumber );
    if( itr == g.vertexMap.end( ) )
        {
            stringstream ss;
            ss << startNumber;
            throw digraphException( ss.str( ) + " is not a valid vertex" );
        }
    
    g.clearAll( );
    Vertex<T> *start = (*itr).second;
    start->dist = 0;
    pq.push( Path<T>( start, 0 ) );

    int nodesSeen = 0;
    for( ; nodesSeen < g.vertexMap.size( ); ++nodesSeen )
    {
        do  // Find an unvisited vertex
        {
            if( pq.empty() )
                return;
            vrec = pq.top( ); pq.pop( );
        } while( vrec.dest->known );

        Vertex<T> *v = vrec.dest;
        v->known = true;

        for( int i = 0; i < v->adj.size(); ++i )
        {
            Edge<T> e = v->adj[ i ];
            Vertex<T> *w = e.dest;
            int cvw = e.cost;

            if( cvw < 0 )
                throw digraphException( "Negative edge seen" );

            // Update w
            if( w->dist > v->dist + cvw )
            {
                w->dist = v->dist + cvw;
                w->prev = v;
                pq.push( Path<T>( w, w->dist ) );
            }
        }
    }
}


// A simple main that reads the file and then computes shortest paths.
int main( int argc, char *argv[] )
{   
    ifstream inFile ( "File.txt" );
    if( !inFile )
    {
        cerr << "Cannot open File.txt" << endl;
        return 1;
    }

    cout << "Reading file... " << endl;
    string oneLine;

    // Read in number of vertices
    getline( inFile, oneLine );
    oneLine.erase( remove( oneLine.begin( ), oneLine.end( ), ' ' ), oneLine.end( ) );
    istringstream iss ( oneLine.substr( oneLine.find( ":" ) + 1 ) );
    int qty;
    iss >> qty;

    // Read in start vertex
    getline( inFile, oneLine );
    oneLine.erase( remove( oneLine.begin( ), oneLine.end( ), ' ' ), oneLine.end( ) );
    istringstream issStart ( oneLine.substr( oneLine.find( ":" ) + 1 ) );
    int startVertex;
    issStart >> startVertex;

    // Read in blankline
    getline( inFile, oneLine );

    // Read the edges; add them to g
    digraph<int> g;
    int source = 0, dest, cost[qty];
    while( getline( inFile, oneLine ) )
    {
        istringstream st( oneLine );

        for( dest = 0; dest < qty; ++dest)
            st >> cost[dest];

        if( st.fail( ) ) continue; 
        else
            for( dest = 0; dest < qty; ++dest )
            {
                if( cost[dest] > 0 )
                    g.addEdge( source, dest, cost[dest] );
            }
        ++source;      
    }
    cout << "File read, digraph constructed" << endl;

    cout << "Writing in adjacency list" << endl;
    ofstream outFile;
    
    outFile.open( "Result.txt", ios::trunc );
    outFile << "Node|\tOut-neighbours" << endl;
    outFile << "-----------------------------------" << endl;
    for( int i = 0; i < qty; ++i )
    {   
        outFile << i <<"\t\t";
        Vertex<int> * u = g.getVertex( i );
        for( int j = 0; j < u->adj.size( ); ++j )
        {
            Edge<int> e = u->adj[ j ];
            Vertex<int> * v = e.dest;
            int cuv = e.cost;
            outFile << v->number << ": " << cuv << "\t";
        }
        outFile << endl;
    }
    
    cout << "Implementing Dijkstra's SSAD algorithm" << endl;
    outFile << endl << endl << "Start vertex is: " << startVertex << endl << endl;
    outFile << "Dest|\tWeight|\tPath" << endl;
    outFile << "-----------------------------------" << endl;
    try
    {
        SSAD( g, startVertex );
        string str;
        for( dest = 0; dest < qty; ++dest )
        {
            str = g.getPath( dest );
            outFile << str;
        }
    }
    catch( const digraphException & e )
    {
        cerr << e.toString( ) << endl;
    }
    outFile.close( );
    cout << "Result file saved";

    return 0;
}