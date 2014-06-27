import java.util.Iterator;
import java.util.List;

import com.orientechnologies.orient.core.record.impl.ODocument;
import com.orientechnologies.orient.core.sql.OCommandSQL;
import com.orientechnologies.orient.core.tx.OTransaction.TXTYPE;
import com.tinkerpop.blueprints.Edge;
import com.tinkerpop.blueprints.Vertex;
import com.tinkerpop.blueprints.impls.orient.OrientGraph;
import com.tinkerpop.blueprints.impls.orient.OrientGraphQuery;

public class OrientLib {
	public static void main(String[] args) {
		OrientLib db = new OrientLib();
		db.InitDB("127.0.0.1", "tin", "2424", "root", "root", "TestVertex", "", "HalfEdge2");
		
		long StartTime = System.currentTimeMillis();
		db.CreateBlankEdge(100000);
		long createblankEdgetim = System.currentTimeMillis();
		System.out.println("Set Blank : " + (createblankEdgetim - StartTime)/1000.0 + "sec");
		db.CreateBlankEdge_DOC(100000);
		//db.UpdateBlankEdge();
		long updateedge = System.currentTimeMillis();
		System.out.println("Set Blank : " + (updateedge - createblankEdgetim)/1000.0 + "sec");
		
		System.out.println("#######db.GetCountOfVertexs()####### : " + db.GetCountOfVertexs());
		System.out.println("#######db.GetCountOfEdges()####### : " + db.GetCountOfEdges());
	}
	
	boolean RemoveDeletedEdge() {
		String query = "delete from "+ m_EdgeClassName + " where deleted = 'yes'";
		m_Graph.getRawGraph().command(new OCommandSQL(query)).execute();
		m_Graph.getRawGraph().commit();

		query = "drop class TIN_BLANK_CLASS";
		m_Graph.getRawGraph().command(new OCommandSQL(query)).execute();
		m_Graph.getRawGraph().commit();
		
		return true;
	}
	
boolean SetRandomVertex(int dataNum){
	m_Graph.setAutoStartTx(true);
	m_Graph.commit();
		try{
			m_Graph.getRawGraph().begin(TXTYPE.NOTX);
			m_Graph.dropVertexType(m_VertexClassName);
			m_Graph.getRawGraph().begin(TXTYPE.NOTX);
			m_Graph.createVertexType(m_VertexClassName);
		} catch (Exception E){
		}
		
		try{
			m_Graph.getRawGraph().begin(TXTYPE.OPTIMISTIC);
			int sq =(int) Math.sqrt(dataNum);
			int idx = 0;
			for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
				for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
					double  x = (double)i;
					double y = (double)j;
					Vertex v = m_Graph.addVertex(m_VertexClassName,m_VertexClassName);
					v.setProperty("x", x);
					v.setProperty("y", y);
					v.setProperty("idx", idx);
					idx++;
					//System.out.println(v.getId());
					if (idx == dataNum) break;
					if (idx % 1000 == 0) {
						m_Graph.commit();
					}
				}
				if (idx == dataNum) break;
			}
			m_Graph.commit();
		} catch(Exception E) {
			return false;
		}
		System.gc();
		return true;
	}


	void UpdateBlankEdge()
	{
		m_Graph.getRawGraph().begin(TXTYPE.NOTX);
		Iterator<Edge> iter = m_Graph.getEdgesOfClass(m_EdgeClassName).iterator();
		int i = 0;
		while (iter.hasNext()){
			Edge e = iter.next();
			int ccw = e.getProperty("ccw");
			e.setProperty("ccw", "-"+ccw);
			if (i%300 == 0)
				m_Graph.commit();
			i++;
		}
		m_Graph.commit();
	}

	String CreateBlankEdge_DOC(int dataNum){
		m_Graph.setAutoStartTx(true);
		m_Graph.commit();
//		m_Graph.getRawGraph().declareIntent(new OIntentMassiveInsert());
		String StartRID ="";
		try{
			m_Graph.getRawGraph().begin(TXTYPE.NOTX);
			for (int i = 0 ; i < dataNum ; i ++) {
				ODocument doc = new ODocument(m_EdgeClassName+"_DOC");
				doc.field("in", m_BlankVertex.getId());
				doc.field("out", m_BlankVertex.getId());
				doc.field("cw", "none");
				doc.field("ccw", "none");
				doc.field("pair", "none");
				doc.field("delete", "none");
				doc.save();
				if (i % 300 == 0) {
					m_Graph.commit();
				}
			}
			m_Graph.commit();
		} catch(Exception E) {
			return "";
		}
		m_Graph.getRawGraph().declareIntent(null);
		
		System.gc();
		
		return StartRID;
	}
	
	String CreateBlankEdge(int dataNum){
		m_Graph.setAutoStartTx(true);
		m_Graph.commit();
//		m_Graph.getRawGraph().declareIntent(new OIntentMassiveInsert());
		String StartRID ="";
		try{
			m_Graph.getRawGraph().begin(TXTYPE.NOTX);
			for (int i = 0 ; i < dataNum ; i ++) {
				Edge e = m_Graph.addEdge(null, m_BlankVertex, m_BlankVertex, m_EdgeClassName);
				e.setProperty("cw", "none");
				e.setProperty("ccw", "none");
				e.setProperty("pair", "none");
				e.setProperty("delete", "none");
				if (i % 300 == 0) {
					m_Graph.commit();
				}
				if(StartRID.length() == 0) {
					StartRID = e.getId().toString();
					//System.out.println("CreateBlankEdge : " + StartRID);
				}
			}
			m_Graph.commit();
		} catch(Exception E) {
			return "";
		}
		m_Graph.getRawGraph().declareIntent(null);
		
		System.gc();
		
		return StartRID;
	}
	public OrientLib() {
		m_Graph = null;
	}
	
	boolean InitDB(String URL, String DBName, String DBPort, String ID, String PW, String VertexClassName, String IndexName, String EdgeClassName) {
		///////////////////////////////////////
		// DB 접속
		String FullPath = "remote:" + URL + "/" + DBName;
		//OGlobalConfiguration.MEMORY_AUTOFREE_HEAP_THRESHOLD.setValue("50%");
//		FullPath = "plocal:/home/iswook/server/orientdb-community-1.7-rc2/databases/tin2";
		
		
		try{
			m_Graph = new OrientGraph(FullPath, ID, PW);
			System.out.println("JAVA ConnectDB OK");
		} catch(Exception e) {
			System.out.println("InitDB() ERROR : " + e.getMessage());
			return false;
		}
		m_VertexClassName = VertexClassName;
		m_EdgeClassName = EdgeClassName;
		m_IndexName = IndexName;
		m_Graph.setAutoStartTx(false);
		m_Graph.commit();
		
//		m_Graph.setAutoStartTx(false);
//		m_Graph.commit();
		
		///////////////////////////////////////
		// DB에 VertexClass가 존재하는지 확인
		if (!_CheckVertexClass()) {
			System.out.println("_CheckVertexClass() ERROR ");
			return false;
		}
		System.out.println("JAVA _CheckVertexClass OK");
		
		if (!_CreateEdgeClass()) {
			System.out.println("_CreateEdgeClass() ERROR ");
			return false;
		}

		System.out.println("JAVA _CreateEdgeClass OK");
		if (!_CreateBlankClass()) {
			System.out.println("_CreateBlankClass() ERROR ");
			return false;
		}
		System.out.println("JAVA _CreateBlankClass OK");

		return true;
	}
	
	private boolean _CreateBlankClass() {
		String BlankClassName = "TIN_BLANK_CLASS";
		try{
			m_Graph.dropVertexType(BlankClassName);
			m_Graph.commit();
		} catch (Exception E){
		}
		
		try{
			m_Graph.createVertexType(BlankClassName);
			m_BlankVertex = m_Graph.addVertex(BlankClassName, BlankClassName);
			m_Graph.commit();
		} catch (Exception E){
			return false;
		}
		return true;
	}
	
	private boolean _CreateEdgeClass() {
		try{
			m_Graph.dropEdgeType(m_EdgeClassName);
		} catch (Exception E){
		}		
		try{
			m_Graph.createEdgeType(m_EdgeClassName, "E");
			
		} catch (Exception E){
			return false;
		}
		return true;
	}
	
	String CreateEdge() {
		Edge e = m_Graph.addEdge(null, m_BlankVertex, m_BlankVertex, m_EdgeClassName);
		m_Graph.getRawGraph().begin(TXTYPE.NOTX);
		e.setProperty("ccw", "0");
		m_Graph.commit();
		System.out.println("JAVA Create Edge : " + e.getId().toString());
		
		return e.getId().toString();
	}
	
	private String _GetEdgeString(Edge e)
	{
		if (e == null) {
			return "";
		}
		
		String retVal; 
		retVal = "@rid:"+e.getId();
		retVal += ",out:"+e.getProperty("out");
		retVal += ",in:"+e.getProperty("in");
		retVal += ",pair:"+e.getProperty("pair");
		retVal += ",ccw:"+e.getProperty("ccw");
		retVal += ",cw:"+e.getProperty("cw");
		
		return retVal;
	}
	
	String GetEdge(String RID) { 
		Edge e = m_Graph.getEdge(RID);
		return _GetEdgeString(e);
	}
	
	private String _GetVertexString(Vertex v) {
		if (v == null) {
			return "";
		}
		String retVal;
		retVal = "@rid:"+v.getId();
		retVal += ",x:"+v.getProperty("x"); 
		retVal += ",y:"+v.getProperty("y");
		retVal += ",halfedge:"+v.getProperty("halfedge");
		
		return retVal;
	}
	String GetVertex(String RID) {
		Vertex v = m_Graph.getVertex(RID);
		
		return _GetVertexString(v);
	}
	String GetVertexFromIndex(int Index){
		if (m_IndexName.length() != 0) {
			String query = "select from index:" + m_IndexName + " skip "+(Index)+" limit 1";
			List<ODocument> doc =  m_Graph.getRawGraph().command(new OCommandSQL(query)).execute();
			
			String vertexRID = doc.get(0).field("rid").toString();
			int startIDX = vertexRID.indexOf('#');
			int endIDX = vertexRID.indexOf('{');
			String rid = vertexRID.substring(startIDX, endIDX);
			return GetVertex(rid);
		}
		
	
		if (m_VertexClassCluster == null) {
			OrientGraphQuery oQuery = (OrientGraphQuery)m_Graph.query();
			Iterator<Vertex> iter = oQuery.labels(m_VertexClassName).vertices().iterator();
			if(iter.hasNext()){
				Vertex v = iter.next();
				String vRID = v.getId().toString(); 
				m_VertexClassCluster = vRID.split(":")[0];
			}
		}
		
		return GetVertex(m_VertexClassCluster + ":" + Index);
	}	
	
	boolean UpdateVertex(String RID, double xPos, double yPos, String RIDHalfEdge) 
	{		
		if (RID.length() < 3) {
			return false;
		}

		Vertex v = m_Graph.getVertex(RID);
		if (v == null) {
			return false;
		}
		v.setProperty("x", xPos);
		v.setProperty("y", yPos);
		v.setProperty("halfedge", RIDHalfEdge);
		m_Graph.commit();

		return true;
	}
	boolean UpdateEdge(String EdgeLists)
	{
		
		String[] EdgeList = EdgeLists.split("/");
		int idx = 0;
		for (String EdgeData : EdgeList) {
			String[] dataLIst = EdgeData.split(";");
			String RID 		= dataLIst[0];
			String RIDOut 	= dataLIst[1];
			String RIDIn 		= dataLIst[2];
			String RIDPair 	= dataLIst[3];
			String RIDCCW 	= dataLIst[4];
			String RIDCW 		= dataLIst[5];
			
			if (RID.length() < 3) {
				return false;
			}
//			String query = "select from "+ RID;
//			System.out.println(query);
//			List<ODocument> docList = m_Graph.getRawGraph().command(new OCommandSQL(query)).execute();
//			ODocument doc = docList.get(0);
//			doc.field("out", RIDOut);
//			doc.field("in", RIDIn);
//			doc.field("pair", RIDPair);
//			doc.field("ccw", RIDCCW);
//			doc.field("cw", RIDCW);
//			doc.save();
			
			Edge e = m_Graph.getEdge(RID);
			if (e == null) {
				return false;
			}
			e.setProperty("out", RIDOut);
			e.setProperty("in", RIDIn);
			e.setProperty("pair", RIDPair);
			e.setProperty("ccw", RIDCCW);
			e.setProperty("cw", RIDCW);
			idx++;
			if (idx % 300 == 0) {
				m_Graph.commit();
			}
		}
		m_Graph.commit();
		return true;
	}
	
	int GetCountOfVertexs(){	
		return (int)m_Graph.countVertices(m_VertexClassName); 
	}
	int GetCountOfEdges() {
		long k = -1;
		try {
			k = m_Graph.getRawGraph().countClass(m_EdgeClassName);
		} catch (Exception e) {
			return -1;
		}
		return (int) k;
	}
	
	boolean DeleteHalfEdge(String RIDs){
		String[] RIDList = RIDs.split(";");
//		System.out.println("Delete RIDList.size : " + RIDList.length);
		m_Graph.query();
		m_Graph.getRawGraph().begin(TXTYPE.NOTX);
		int idx = 0;
		for (String RID : RIDList) {
			Edge e = m_Graph.getEdge(RID);
	//		System.out.println("JAVA Remove Edge : " + RID + "e.RID : " + e.getId());
			e.setProperty("deleted", "yes");
//			m_Graph.removeEdge(e);
			idx ++;
			if (idx % 300 == 0 ){
				m_Graph.commit();
			}
		}
		m_Graph.commit();
		return true;
	}
	
	
	
	private boolean _CheckVertexClass() {
		try {
			m_Graph.countVertices(m_VertexClassName);
		} catch (Exception E){
			return false;
		}
			
		return true;
	}

	///////////////////////////////////
	// Member 변수 
	///////////////////////////////////
	private String 		m_VertexClassName;
	private String 		m_EdgeClassName;	
	private OrientGraph 	m_Graph;
	private Vertex		m_BlankVertex;
	private String 		m_VertexClassCluster;
	private String 		m_IndexName;
}
